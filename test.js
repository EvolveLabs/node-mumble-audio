var openal = require('./openal');
var util = require('util');

var speaker = new openal.Device();
var source = new openal.CaptureDevice();

source.on('data', function (data) {
	speaker.write(data);
})

source.start();
speaker.play();
