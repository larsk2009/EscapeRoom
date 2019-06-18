var canvas = document.getElementById("gameCanvas");
canvas.width = canvas.offsetWidth;
canvas.height = canvas.offsetHeight;
var context = canvas.getContext("2d");


//Draw the sprite
var sprite_0 = new Image();
sprite_0.src = "images/avery walking-a.svg";
var sprite_1 = new Image();
sprite_1.src = "images/avery walking-b.svg";
var sprite_2 = new Image();
sprite_2.src = "images/avery walking-c.svg";
var sprite_3 = new Image();
sprite_3.src = "images/avery walking-d.svg";

var img = sprite_0;

const start_y = 400;
const start_x = 0;
var x = start_x;
var y = start_y;

const rectangle_x = 600;
const rectangle_y = 400;
const rectangle_width = 500;
const rectangle_height = 400;

var tree = new Tree(CommandEnum.CODE);
var currentPlayNode = tree._root; //Node that the game is currently at
var hasNewTree = false; //When the three has been (re)build

var treeDepth = 0; //How deep into the tree we are
var InTreeLocation = []; //Where in the branch of the tree we are
InTreeLocation.push(0);

var requestingAnimation = true;

Update();

var connection = new signalR.HubConnectionBuilder().withUrl("/resetdevicehub").build();

connection.on("ResetReceived", function () {
        location.reload();
    });

connection.start();


function Update() {
    context.clearRect(0, 0, canvas.width, canvas.height);
    context.fillStyle = "#d90eef";
    context.fillRect(rectangle_x, rectangle_y, rectangle_width, rectangle_height);

    let noGravity = false;

    if (hasNewTree) {
        if (currentPlayNode.method == CommandEnum.CODE || currentPlayNode.method == CommandEnum.REPEAT) {
            TraverseIntoTree();
        } else if (currentPlayNode.method == CommandEnum.IF) {
            const splits = currentPlayNode.data.split('==');
            const variable = splits[0].trim();
            const constant = splits[1];
            let leftSide = null;
            switch (variable) {
                case "x":
                    leftSide = x;
                    break;
            }
            if (leftSide == Number(constant)) {
                TraverseIntoTree();
            } else {
                //Move to sibling or up tree
                const length = currentPlayNode.parent.children.length;
                const index = InTreeLocation[treeDepth];
                if (index < length - 1) {
                    currentPlayNode = currentPlayNode.parent.children[index + 1];
                    InTreeLocation[treeDepth]++;
                } else {
                    TraverseOutTree();
                }
            }
        } else if (currentPlayNode.method.includes("beweeg")) {
            const amount = currentPlayNode.method.split('(')[1].replace(')', '').replace(';', '');
            Move(Number(amount));
            //Move to sibling or up tree
            const length = currentPlayNode.parent.children.length;
            const index = InTreeLocation[treeDepth];
            if (index < length - 1) {
                currentPlayNode = currentPlayNode.parent.children[index + 1];
                InTreeLocation[treeDepth]++;
            } else {
                TraverseOutTree();
            }
        } else if (currentPlayNode.method.includes("spring")) {
            if (typeof Update.jumpAmount == 'undefined') {
                // It has not... perform the initialization
                Update.jumpAmount = 0;
            }

            noGravity = true;

            let amount = 0;
            if (Update.jumpAmount === 0) {
                let amount = currentPlayNode.method.split('(')[1].replace(')', '').replace(';', '');
                Update.jumpAmount = amount;
            } else {
                if (Update.jumpAmount < 5) {
                    amount = Update.jumpAmount;
                    Update.jumpAmount = 0;
                } else {
                    amount = 5;
                    Update.jumpAmount -= 5;
                }
            }

            Jump(Number(5));
            //Move to sibling or up tree
            const length = currentPlayNode.parent.children.length;
            const index = InTreeLocation[treeDepth];
            if (Update.jumpAmount === 0) {
                if (index < length - 1) {
                    currentPlayNode = currentPlayNode.parent.children[index + 1];
                    InTreeLocation[treeDepth]++;
                } else {
                    TraverseOutTree();
                }
            }
        }
    }

    if (!noGravity) {
        DoGravity();
    } else {
        noGravity = false;
    }

    context.drawImage(img, x, y, img.width * 1.55, img.height * 1.55);


    context.fillStyle = "#ff1700";
    context.strokeStyle = "#000000";
    context.fillRect(canvas.width - 205, canvas.height / 2 - 150, 200, 100);
    context.strokeRect(canvas.width - 205, canvas.height / 2 - 150, 200, 100);

    context.fillStyle = "#000000";
    context.font = "30px Arial";
    context.fillText("FINISH!", canvas.width - 150, canvas.height / 2 - 90);

    context.strokeRect(canvas.width - 5, canvas.height / 2 - 150, 1, 200);

    SelectNextSprite();

    if (CheckWinCondition()) {
        //Show win
        context.fillStyle = "#4dee00";
        context.font = "30px Arial";
        context.fillText("YOU WIN!", canvas.width / 2 - 100, canvas.height / 2 - 100);
        GetDisplayNumber(function (data) {
            context.fillStyle = "#4dee00";
            context.font = "30px Arial";
            context.fillText(data, canvas.width / 2 - 100, canvas.height / 2 - 50);
        });
    } else if (CheckLoseCondition()) {
        //Show lose
        context.fillStyle = "#ee002b";
        context.font = "30px Arial";
        context.fillText("YOU LOSE!", canvas.width / 2 - 100, canvas.height / 2 - 100);
    } else {
        if (requestingAnimation) {
            requestAnimationFrame(Update);
        }
    }
}

function DecodeInput() {
    const textArea = document.getElementById("codeArea");
    const text = textArea.value;
    const split = text.split('\n');
    const filtered = split.filter(function (el) {
        return el != null && el != ' ' && el != "";
    });
    const length = filtered.length;

    //reset canvas
    x = start_x;
    y = start_y;
    tree = new Tree(CommandEnum.CODE);
    var currentRoot = tree._root;
    hasNewTree = false;

    for (let i = 0; i < length; i++) {
        if (filtered[i].includes("herhaal")) {
            currentRoot.children.push(new Node(CommandEnum.REPEAT));
            currentRoot.children.last().parent = currentRoot;
            currentRoot = currentRoot.children.last();
        } else if (filtered[i].includes("als")) {
            currentRoot.children.push(new Node(CommandEnum.IF));
            currentRoot.children.last().data = GetValueFromFunction(filtered[i]);
            currentRoot.children.last().parent = currentRoot;
            currentRoot = currentRoot.children.last();
        } else if (filtered[i].includes("beweeg")) {
            currentRoot.children.push(new Node(filtered[i]));
            currentRoot.children.last().parent = currentRoot;
        } else if (filtered[i].includes("spring")) {
            currentRoot.children.push(new Node(filtered[i]));
            currentRoot.children.last().parent = currentRoot;
        } else if (filtered[i] == "}") {
            currentRoot = currentRoot.parent;
        }
    }
    currentPlayNode = tree._root;
    treeDepth = 0;
    InTreeLocation = [];
    InTreeLocation.push(0);

    hasNewTree = true;
    requestingAnimation = true;
    Update.jumpAmount = undefined;

    Update();
}

function GetDisplayNumber(callback) {
    $.get("home/getdisplaynumber", function (data) {
        callback(data);
    });
}

function SelectNextSprite() {
    if (typeof SelectNextSprite.counter == 'undefined') {
        SelectNextSprite.counter = 0;
    }
    SelectNextSprite.counter++;
    if (SelectNextSprite.counter > 15) {
        SelectNextSprite.counter = 0;
        if (img == sprite_0) {
            img = sprite_1;
        } else if (img == sprite_1) {
            img = sprite_2;
        } else if (img == sprite_2) {
            img = sprite_3;
        } else if (img == sprite_3) {
            img = sprite_0;
        }
    }
}

function DoGravity() {
    if ((y < start_y && (x + sprite_1.width) < rectangle_x) || (((y + sprite_1.height + 110) < rectangle_y) && (x + sprite_1.width) > rectangle_x)) {
        y += 2.5;
    }
}

/**
 * @return {boolean}
 */
function CheckWinCondition() {
    return x >= canvas.width - 100 && y <= 105;
}

/**
 * @return {boolean}
 */
function CheckLoseCondition() {
    let checkIsInBox = x + sprite_1.width >= rectangle_x && y + sprite_1.height + 100 >= rectangle_y;
    let HasHitHead = y <= 0;
    return checkIsInBox || HasHitHead;

}

//Gets the value that is in between the brackets of a function
/**
 * @return {string}
 */
function GetValueFromFunction(text) {
    return text.split('(')[1].replace(')', '').replace(';', '').replace('{', '');
}

function TraverseIntoTree() {
    currentPlayNode = currentPlayNode.children[0];
    treeDepth++;
    InTreeLocation[treeDepth] = 0;
}

function TraverseOutTree() {
    if (currentPlayNode.parent.method == CommandEnum.REPEAT) {
        currentPlayNode = currentPlayNode.parent;
        InTreeLocation = [];
        InTreeLocation.push(0);
    } else {
        InTreeLocation[treeDepth] = 0;
        treeDepth--;
        const index = InTreeLocation[treeDepth];
        const length = currentPlayNode.parent.parent.children.length;
        if (index < length - 1) {
            currentPlayNode = currentPlayNode.parent.parent.children[index + 1];
        } else {
            currentPlayNode = currentPlayNode.parent;
            TraverseOutTree();
        }
    }
}

function Move(steps) {
    x += steps;
}

function Jump(height) {
    if (height > 500) {
        height = 500;
    }
    y -= height; //Decrease y because origin is in the top left corner of the canvas 
    // so jumping actually decreases the distance from the origin instead of increasing it
}