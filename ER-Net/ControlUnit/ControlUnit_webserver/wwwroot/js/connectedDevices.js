"use strict";

var connection = new signalR.HubConnectionBuilder().withUrl("/devicesHub").build();

connection.on("AddDevice", function (devices) {
    var list = document.getElementById("devicesList");
    list.innerHTML = "";
    for(var i = 0; i < devices.length; i++) {
        var li = document.createElement("li");
        li.textContent = devices[i];
        list.appendChild(li);
    }
});

connection.on("SolutionChanged", function(solution) {
    var solutionParagraph = document.getElementById("solution");
    solutionParagraph.textContent = solution;
});

connection.start();

document.getElementById("resetBtn").addEventListener("click", function (ev) { 
    connection.invoke("Reset").catch(function(err) {
        return console.error(err.toString());
    });
    event.preventDefault();
});
