<?php
session_start();
?>
<!DOCTYPE html>
<html>
<body>

<?php
// remove all session variables
session_unset(); 

// destroy the session 
session_destroy(); 
?>
<meta http-equiv="refresh" content="0;URL=login.php">

</body>
</html>
