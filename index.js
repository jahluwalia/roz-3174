const j5 = require('johnny-five');
const board = new j5.Board({
  port: "/dev/tty.usbmodem213301"
});

board.on('ready', function() {
  console.log("Roz is ready!");
  const led = new j5.Led(13);
//   led.blink(500);
    led.strobe();
});