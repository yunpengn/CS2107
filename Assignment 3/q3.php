<?php

$state = 0;


if (!isset($included)) {
    $state = -1;
}

function print_flag() {
    $flag = "cs2107{7143150b53d6e4d4bdeb3eb480195c54}"; 
    echo $flag;
}
?>

<?php if ($state == -1) : ?>
<html>
<head>
  <meta charset="UTF-8">
  <title>Error</title>
</head>
<body>
  <h2>Error</h2>
  <p>This file only works when it is included within another file.</p>
</body>
</html>
<?php elseif ($state == 0) : ?>
<p>And the flag is cs2107{..... </p>
<p>Just kidding. The flag is hiding in the php code... not like it's possible to view php source right?</p>
<?php endif; ?>
