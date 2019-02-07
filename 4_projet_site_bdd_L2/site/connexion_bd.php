



<?php
 
$user = 'amalek';
$pass = 'chocolat';

try {
    $cnx = new PDO('pgsql:host=sqletud;dbname=amalek_db', $user, $pass);			         // A COMPLETER  
}
catch (PDOException $e) {
    echo "ERREUR : La connexion à la base de donnée a échouée";

 /* Affiche des informations potentiellement confidentielles a éviter de le faire pour un site en production.*/
}


?>

