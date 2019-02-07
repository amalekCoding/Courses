<?php 

// Execute un refresh de la page en PHP

$delai=1; 
$url="peut_cor.php";
include($url);
header("Refresh: 0;url=$url");

?>
