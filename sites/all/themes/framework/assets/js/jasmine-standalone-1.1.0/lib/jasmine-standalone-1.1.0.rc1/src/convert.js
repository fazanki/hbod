describe( "Convert library", function () {  
    describe( "distance converter", function () {  
  
        });  
  
        describe( "volume converter", function () {  
  
    });  
});  

describe( "distance converter", function () {  
    it("converts inches to centimeters", function () {  
        expect(Convert(12, "in").to("cm")).toEqual(30.48);  
    });  
  
    it("converts centimeters to yards", function () {  
        expect(Convert(2000, "cm").to("yards")).toEqual(21.87);  
    });  
});  

describe( "volume converter", function () {  
    it("converts litres to gallons", function () {  
        expect(Convert(3, "litres").to("gallons")).toEqual(0.79);  
    });  
  
    it("converts gallons to cups", function () {  
        expect(Convert(2, "gallons").to("cups")).toEqual(32);  
    });  
});  


it("throws an error when passed an unknown from-unit", function () {  
    var testFn = function () {  
        Convert(1, "dollar").to("yens");  
    }  
    expect(testFn).toThrow(new Error("unrecognized from-unit"));  
});  
  
it("throws an error when passed an unknown to-unit", function () {  
    var testFn = function () {  
        Convert(1, "cm").to("furlongs");  
    }  
    expect(testFn).toThrow(new Error("unrecognized to-unit"));  
});  


function Convert(number, fromUnit) {  
    var conversions = {  
            distance : {  
                meters : 1,  
                cm     : 0.01,  
                feet   : 0.3048,  
                inches : 0.0254,  
                yards  : 0.9144  
            },  
            volume : {  
                liters : 1,  
                gallons: 3.785411784,  
                cups   : 0.236588236  
            }  
        },  
        betweenUnit = false,  
        type, unit;  
  
    for (type in conversions) {  
        if (conversions(type)) {  
            if ( (unit = conversions[type][fromUnit]) ) {  
                betweenUnit = number * unit * 1000;  
            }  
        }  
    }  
  
    return {  
        to : function (toUnit) {  
            if (betweenUnit) {  
                for (type in conversions) {  
                    if (conversions.hasOwnProperty(type)) {  
                        if ( (unit = conversions[type][toUnit]) ) {  
                            return fix(betweenUnit / (unit * 1000));  
                        }  
                    }  
                }  
                throw new Error("unrecognized to-unit");  
            } else {  
                throw new Error("unrecognized from-unit");  
            }    
  
            function fix (num) {  
                return parseFloat( num.toFixed(2) );  
            }  
        }  
    };  
}  
