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

connection.on("TimerUpdate", function(timerValue) {
    var timerElement = document.getElementById("timeHeader");
    timerElement.textContent = timerValue;
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