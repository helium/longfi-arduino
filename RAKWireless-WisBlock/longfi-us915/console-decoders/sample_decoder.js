// Helium console decoder
// This is a very simple decoder for testing only. modify to suit your
// payload  needs

function Decoder(bytes, port) {
  var custom_msg={}; 
  try{
      var result = String.fromCharCode.apply(null, bytes);
      custom_msg.received_payload = result;
      return custom_msg;
  } catch (err) {
      return 'Decoder: ' + err.name + " : " + err.message;; 
  }
}
