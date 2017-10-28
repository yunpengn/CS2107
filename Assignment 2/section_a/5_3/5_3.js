var input = "T:743deh {6408ecffea2Fsbefa7l29b513a101fe6g031f2}";

console.log("The length of the string is " + input.length);
console.log("Separate into a 7-by-6 matrix:");

var matrix_result = "";
for (var i = 0; i < 7; i++) {
	matrix_result += input.substring(7 * i, 7 * i + 7) + "\n";
}
console.log(matrix_result);

var decrypt_result = "";
for (var m = 0; m < 7; m++) {
	for (var n = 0; n < 7; n++) {
		decrypt_result += input.charAt(m + 7 * n);
	}
}
console.log(decrypt_result);