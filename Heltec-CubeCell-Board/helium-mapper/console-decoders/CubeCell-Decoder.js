// This Helium Network Console decoder is intended to be used with the 
// Helium Mapper and Helium Cargo mapping efforts

function Decoder(bytes, port) {
  return {
    latitude:
      ((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]) / 1E7,
    longitude:
      ((bytes[4] << 24) | (bytes[5] << 16) | (bytes[6] << 8) | bytes[7]) / 1E7,
    altitude:
      0,
    sats:
      (bytes[9]),
    speed:
      (((bytes[8]))/1.609).toFixed(2), 
    battery:
      (((bytes[10])*0.2)/10).toFixed(2)
    
  };
}
