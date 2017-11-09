var input = "49 59 18 1b 1a 1d 51 19 52 1b 4c 75 49 1e 44 75 59 5e 1a 58 19 75 47 19 59 59 1e 4d 19 59 75 5e 1a 1a 57 20";
var splitted = input.split(" ");

var char_codes = [];

for (var i = 0; i < splitted.length; i++) {
	char_codes[i] = parseInt(splitted[i], 16);
}

var xor_codes = [];

for (i = 0; i < splitted.length; i++) {
	xor_codes[i] = char_codes[i] ^ 42;
}

var result = "";

for (i = 0; i < splitted.length; i++) {
	result += String.fromCharCode(xor_codes[i]);
}

console.log(result);
