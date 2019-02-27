var canvas = document.getElementById("gameCanvas");
canvas.width = canvas.offsetWidth;
canvas.height = canvas.offsetHeight;
var context = canvas.getContext("2d");


//Draw the sprite
var img = new Image();
img.src = "images/avery walking-b.svg";

const start_y = 400;
const start_x = 0;
var x = start_x;
var y = start_y;

const rectangle_x = 600;
const rectangle_y = 400;
const rectangle_width = 500;
const rectangle_height = 400;

var tree = new Tree(CommandEnum.CODE);
var currentPlayNode = tree._root; //Note that the game is currently at
var hasNewTree = false; //When the three has been (re)build

var treeDepth = 0; //How deep into the tree we are
var InTreeLocation = []; //Where in the branch of the tree we are
InTreeLocation.push(0);

Update();

function Update() {
    context.clearRect(0, 0, canvas.width, canvas.height);
    context.fillStyle = "#d90eef";
    context.fillRect(rectangle_x, rectangle_y, rectangle_width, rectangle_height);

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
            const amount = currentPlayNode.method.split('(')[1].replace(')', '').replace(';', '');
            Jump(Number(amount));
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
    }

    context.drawImage(img, x, y, 75, 300);

    if (CheckWinCondition()) {
        //Show win
        context.fillStyle = "#4dee00";
        context.font = "30px Arial";
        context.fillText("YOU WIN!", canvas.width / 2 - 100, canvas.height / 2 - 100);
    } else if (CheckLoseCondition()) {
        //Show lose
        context.fillStyle = "#ee002b";
        context.font = "30px Arial";
        context.fillText("YOU LOSE!", canvas.width / 2 - 100, canvas.height / 2 - 100);
    } else {
        requestAnimationFrame(Update);
    }
}

function DecodeInput() {
    tree = new Tree(CommandEnum.CODE);
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
    hasNewTree = true;
    Update();
}

/**
 * @return {boolean}
 */
function CheckWinCondition() {
    return x >= canvas.width - 100 && y <= 100;
}

/**
 * @return {boolean}
 */
function CheckLoseCondition() {
    return x + img.width >= rectangle_x && y + img.height + 100 >= rectangle_y;

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
    y -= height; //Decrease y because origin is in the top left corner of the canvas 
                 // so jumping actually decreases the distance from the origin instead of increasing it
}