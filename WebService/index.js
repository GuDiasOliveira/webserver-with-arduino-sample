var SerialPort = require('serialport');
var serialPort = new SerialPort('COM3', {
	baudrate: 9600
});

var express = require('express');
var app = express();

app.get('/num/:n', function (req, res) {
	var n = parseInt(req.params.n);
	serialPort.write('num ' + n);
	res.end('Ok', 'text/plain');
});

app.get('/led/:color/:val', function (req, res) {
	var color = req.params.color;
	var val = parseInt(req.params.val);
	var msg = 'Ok';
	if (color == 'red')
		serialPort.write('led r ' + val);
	else if (color == 'yellow')
		serialPort.write('led y ' + val);
	else if (color == 'green')
		serialPort.write('led g ' + val);
	else if (color == 'white')
		serialPort.write('led w ' + val);
	else
		msg = 'Invalid led color';
	res.end(msg, 'text/plain');
});

serialPort.on('open', function() {
	console.log('Connected to Arduino');
	app.listen(8082, function() {
		console.log('Web server started');
	})
});