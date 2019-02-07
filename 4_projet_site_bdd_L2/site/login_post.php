<?php
session_start(); 

// on vérifie que les données du formulaire sont présentes
if (isset($_POST['login']) && isset($_POST['mdp'])) {
    require("connexion_bd.php");
    
    $requete = "SELECT * FROM authentification WHERE Login=? AND motdepasse=?";
    $resultat = $cnx->prepare($requete);
    $login = $_POST['login'];
    $mdp = $_POST['mdp'];
    echo $mdp;
    $resultat->execute(array($login, md5($mdp)));
    if ($resultat->rowCount() == 1) {
        // l'utilisateur existe dans la table
        // on ajoute ses infos en tant que variables de session
        $_SESSION['login'] = $login;
        $_SESSION['mdp'] = $mdp;
        $_SESSION['connecte'] =1;
        
        
        
        
        $res = $cnx->query("SELECT type FROM authentification WHERE login ='".$login."'");
        $type = $res->fetch();
        echo $type[0];
        

        $type[0]=preg_replace('/\s/', '', $type[0]);
        echo $type[0];
        $_SESSION['type'] = $type[0];
        
        if($type[0] == 'correcteur'){
			$res = $cnx->query("SELECT idsurv_cor FROM surveillant_correcteur WHERE login = '".$_SESSION['login']."'");
			$numpersonne = $res->fetch();
		}
		
		
		
		
		$_SESSION['numpersonne'] = $numpersonne[0];		
        // cette variable indique que l'authentification a réussi
        $authOK = true;
        
    }
}
include("menu.php");
?>

<!DOCTYPE HTML>
<html>
	<head>
		<meta charset="UTF-8" />
		<title>Résultat de l'authentification</title>

	<style type="text/css">
p{
    margin-left: auto;
    margin-right: auto;
    margin-top: 50px;
	width: 600px;
text-align: center;
border:10px solid gray;
}
	</style>

	</head>
	<body>
		<p>Résultat de l'authentification</p>
		<?php
		if (isset($authOK)) {
			echo "<p>Vous avez été reconnu(e) en tant que : " . $login . "<br>";
			echo '<a href="index.php">Poursuivre vers la page d\'accueil</a> <br>';
			echo 'session type = '.$_SESSION['type'] ;
		}
		else { ?>
			
		<script type="text/javascript">
		document.location.href="login.php?no=1"
		</script>


		<?php } ?>
	</body>
</html>
