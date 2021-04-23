// DataCake Decoder
// This decoder is an example which can be used to decode the cubecell payload
// to be able to use it as a tracking device to track position on a mobile
// phone or tablet using the DataCake cloud services

function Decoder(bytes, port) {
    // Decode an uplink message from a buffer
    // (array) of bytes to an object of fields.

    datacakeFields = [

          {
           field: "SATS",
           value: bytes[9]
        },
           {
            field: "SPEED",
            value: (((bytes[8]))/1.609).toFixed(2)
        },
           {
            field: "BATTERY",
            value: (((bytes[10])*0.2)/10).toFixed(2)
        },
    ];
	
    var lat = ((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]) / 1E7;
	var lon = ((bytes[4] << 24) | (bytes[5] << 16) | (bytes[6] << 8) | bytes[7]) / 1E7;
    
    if ((lat < 90) && (lat > -90)) {
        if ((lon < 90) && (lon > -90)) {
            if ((lat !== 0) && (lon !==0)) {
                datacakeFields.push({
                    field: "LOCATION",
                    value: "(" + lat + "," + lon + ")"
                });
            }        
       }
    }
     else {
        console.log("No GPS Lock - Skipping Location")
    }
    
    return datacakeFields;
}
