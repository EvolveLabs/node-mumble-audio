var openal = require('./openal');
var util = require('util');

//var context = new openal.Context();
//var mumble = ...;
//mumble.on('voice', function (data) {
//	context.play(data);	
//})

var source = new openal.CaptureDevice();
source.on('data', function (data) {
	//mumble.out_stream.write(data);
	console.log(util.inspect(data));

})
source.start();
