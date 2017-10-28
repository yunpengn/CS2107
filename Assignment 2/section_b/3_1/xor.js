var str = "Take every byte xor it with forty-two you will get the flag";
var char_codes = [];

for (var i = 0; i < str.length; i++) {
	char_codes[i] = str.charCodeAt(i);
}

var xor_codes = [];

for (i = 0; i < str.length; i++) {
	xor_codes[i] = char_codes[i] ^ 42;
}

var result = "";

for (i = 0; i < str.length; i++) {
	result += String.fromCharCode(xor_codes[i]);
}

result;