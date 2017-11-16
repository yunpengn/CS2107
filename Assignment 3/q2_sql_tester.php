<!DOCTYPE html>
<html>
<head>
	<title>CS2107 Assignment Test</title>

	<!-- Character setting, icon setting and mobile compatibility -->
	<meta charset="utf-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">

	<!-- Latest compiled and minified CSS -->
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u" crossorigin="anonymous">

	<!-- jQuery file, must be put before Bootstrap javascript file -->
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>

	<!-- Latest compiled and minified JavaScript -->
	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js" integrity="sha384-Tc5IQib027qvyjSMfHjOMaLkfuWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa" crossorigin="anonymous"></script>
</head>
<body>
<?php
// Create connection to the database or report error.
$db = mysqli_connect(DB_SERVER, DB_UNAME, DB_PWORD, DB_NAME) or die("Cannot connect to the database." . mysqli_connect_error($db));

$sql = "";
$username = "";
$password = "";

// To record the most recent SQL query record.
if ($_POST) {
	$username = $_POST["username"];
	$password = $_POST["password"];
	$sql = "SELECT * FROM users WHERE username = '$username' AND password = '$password'";

	$result = mysqli_query($db, $sql) or die ("Query is not successfuly.");

	while($row = mysqli_fetch_assoc($result)) {
		if(mysqli_num_rows($result) == 1) {
			if($row['password'] == $password) {
				if($row['user_lvl'] >= 9000) {
					echo "Congratulations!";
				}
			}
		}
	}
}
?>

<div class="container">
	<br><br>
	<div class="col-xs-12 col-sm-10 col-sm-offset-1 col-md-6 col-md-offset-3 col-lg-8 col-lg-offset-2">
		<form role="form" method="post" action="sql_tester.php">
			<div id="error_message" class="">
				<?php echo $sql; ?>
				<br><br>
			</div>

			<div class="form-group">
				<label for="username">Username</label>
				<input type="text" name="username" class="form-control" id="username" value="<?php echo $username; ?>" placeholder="Type username" accesskey="u" tabindex="1" required autofocus>
			</div>

			<div class="form-group">
				<label for="password">Password</label>
				<input type="text" name="password" class="form-control" id="password" value="<?php echo $password; ?>" placeholder="Type password" accesskey="p" tabindex="2" required>
			</div>

			<button type="submit" class="btn btn-primary">Sign in</button>
		</form>
	</div>
</div>
</body>
</html>