var openal = require('./openal');

var context = new openal.Context();

// Get default devices
var device = new openal.OutputDevice();
var source = new openal.CaptureDevice();

// Optionally capture wav data from a file instead
//var data = new openal.WavData(path.join(__dirname, '..', 'sounds', 'synth.wav'));
//var source = new openal.Source( data );

context.setOutput( device );
context.setSource( captureDevice );
context.play();
