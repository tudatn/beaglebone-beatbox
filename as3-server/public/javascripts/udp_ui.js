"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();
var errorTimerProgram;
var errorTimerServer;
$(document).ready(function() {

	$('#modeNone').click(function(){
		sendPrimeCommand("modeNone");
	});
	$('#modeRock1').click(function(){
		sendPrimeCommand("rockBeat");
	});
	$('#modeRock2').click(function(){
		sendPrimeCommand("myBeat");
	});
	$('#volumeDown').click(function(){
		sendPrimeCommand("volumeDown");
	});
	$('#volumeUp').click(function(){
		sendPrimeCommand("volumeUp");
	});
	$('#tempoDown').click(function(){
		sendPrimeCommand("tempoDown");
	});
	$('#tempoUp').click(function(){
		sendPrimeCommand("tempoUp");
	});
	$('#hihat').click(function(){
		sendPrimeCommand("Hihat");
	});
	$('#snare').click(function(){
		sendPrimeCommand("Snare");
	});
	$('#base').click(function(){
		sendPrimeCommand("Base");
	});

	// get proc/uptime
	setInterval(function() {
		socket.emit('proc', 'uptime');
	}, 1000);
	
	// get update from BBB
	setInterval(function() {
		socket.emit('prime', 'update');
	}, 500);

	socket.on('commandReply', function(result) {
		clearTimeout(errorTimerProgram);
//		console.log("Reply message: " + result);
		var resultData = result.split(" ");
		$('#error-box').hide();
		$('#modeid').html(resultData[1]);
		$('#volumeid').val(resultData[3]);
		$('#tempoid').val(resultData[5]);
	});

	socket.on('fileContents', function(result) {
//		console.log(result.contents);
		var totalSeconds = parseInt(result.contents);
		var hrs = Math.floor(totalSeconds / (60 * 60));
		var mins = Math.floor((totalSeconds % (60 * 60)) / 60);
		var secs = (totalSeconds % (60 * 60)) % 60;
		$('#status').html(hrs + ':' + mins + ':' + secs + ' (H:M:S)');
	})

	socket.on('programError', function(result) {
//		console.log(result);
		$('#error-box').show();
		$('#error-text').html(result);
		clearTimeout(errorTimerProgram);
		errorTimerProgram = setTimeout(function() {
			$('#error-box').hide();
			}, 10000);
	});
});

function sendPrimeCommand(message) {
	 // reset the timeout if the user keeps pressing buttons
	clearTimeout(errorTimerProgram);
	clearTimeout(errorTimerServer);

	// check within 1s if no response from server
	errorTimerProgram = setTimeout(function() {
		$('#error-box').show();
		$('#error-text').html("SEVER ERROR: SERVER is OFFLINE!");
	}, 1000);

	// hide error (10s)
	errorTimerServer = setTimeout(function() {
		$('#error-box').hide();
	}, 10000);
	socket.emit('prime', message);
};
