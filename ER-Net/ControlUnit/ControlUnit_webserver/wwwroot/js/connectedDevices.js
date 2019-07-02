"use strict";

var connection = new signalR.HubConnectionBuilder().withUrl("/devicesHub").build();

connection.on("AddDevice", function (devices) {
    var list = document.getElementById("devicesList");
    list.innerHTML = "";
    for(var i = 0; i < devices.length; i++) {
        var device = devices[i];
        
        var li = document.createElement("li");
        var NameParagraph = document.createElement("p");
        var NumberParagraph = document.createElement("p");
        
        NameParagraph.className = "deviceName";
        NumberParagraph.className = "displayNumber"
        
        var splits = device.split("\t");
        NameParagraph.textContent = splits[0];
        NumberParagraph.textContent = splits[1];
        
        li.appendChild(NameParagraph);
        li.appendChild(NumberParagraph);
        list.appendChild(li);
    }
});

connection.on("SolutionChanged", function(solution) {
    var solutionParagraph = document.getElementById("solution");
    solutionParagraph.textContent = solution;
});

connection.on("TimerUpdate", function(timerValue) {
    var timerElement = document.getElementById("timeHeader");
    timerElement.textContent = timerValue;
});

connection.on("StatusChanged", function(status) {
    var statusElement = document.getElementById("statusHeader");
    statusElement.textContent = status;
});

connection.start();

document.getElementById("resetBtn").addEventListener("click", function (ev) {
    connection.invoke("Reset").catch(function(err) {
        return console.error(err.toString());
    });
    ev.preventDefault();
    $("#startBtn").attr("disabled", false);
    $("#stopBtn").attr("disabled", true);
});

document.getElementById("startBtn").addEventListener("click", function (ev) {
    connection.invoke("StartTimer").catch(function(err) {
        return console.error(err.toString());
    });
    ev.preventDefault();
    $("#startBtn").attr("disabled", true);
    $("#stopBtn").attr("disabled", false);
    $("#resetBtn").attr("disabled", true);
});

document.getElementById("stopBtn").addEventListener("click", function (ev) {
    connection.invoke("StopTimer").catch(function(err) {
        return console.error(err.toString());
    });
    ev.preventDefault();
    $("#stopBtn").attr("disabled", true);
    $("#startBtn").attr("disabled", false);
    $("#resetBtn").attr("disabled", false);
    
});