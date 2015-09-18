
var mumble = require('electron-mumble');
var audio = require('./index');
var util = require('util');

console.log(util.inspect(audio));

var devices = audio.devices();
console.log(require('util').inspect(devices));


var output;
var capture;
devices.forEach(function (device) {
    if (device.kind == "output" && device.default) {
        output = device;
    }
    if (device.kind == "capture" && device.default) {
        capture = device;
    }
})

console.log("total device count: " + devices.length);
console.log("output: " + output.name);
console.log("capture: " + capture.name);

var speaker = new audio.PlaybackDevice(output);
var source = new audio.CaptureDevice(capture);
var out_stream = null;

console.log( 'Connecting to: ' +  process.env.MUMBLE_SERVER );
mumble.connect( process.env.MUMBLE_SERVER, {}, function ( error, connection ) {
    if( error ) { throw new Error( error ); }

    console.log( 'Connected.' );
    console.log( 'Authenticating...' );

    out_stream = connection.inputStream({sampleRate: 48000});

    connection.authenticate( process.env.MUMBLE_USER, process.env.MUMBLE_PASS );
    connection.on( 'initialized', onInit );
    connection.on( 'voice', onVoice );
});

var onInit = function() {
    console.log( 'Authenticated.' );

    source.on('data', function (data) {
    	out_stream.write(data);
    })
    source.start();

    // Connection is authenticated and usable.
    speaker.start();
};

var onVoice = function( pcmData ) {
    speaker.write(pcmData);
}
