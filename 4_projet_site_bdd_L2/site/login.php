<?php
session_start();

if (isset($_SESSION['login'])){ 				/* Si deja connecter on le redirige */
	header('Location: index.php'); 
	die();
} 

include("menu.php");
include("connexion_bd.php");
?>


<!DOCTYPE html>
<html>
    <head>
		
        <meta charset="utf-8">
        <title>Convocation</title>
        <link rel="stylesheet" href="style.css">


		<style>
			#main {
				position: absolute; /* postulat de départ */
				top: 50%; left: 50%; /* à 50%/50% du parent référent */
				transform: translate(-50%, -50%); /* décalage de 50% de sa propre taille */
			}

			h2 {
				background-color:#FEFFED;
				text-align: center;
				border-radius:10px 10px 0 0;
				margin:-10px -40px;
				padding:15px;
			}

			#login {
				width:400px;
				float:left;
				border-radius:10px;
				font-family:raleway;
				border:2px solid #ccc;
				padding:10px 40px 25px;
				margin-top:10px
			}
			input[type=text],input[type=password] {
				width:400px;
				padding:10px;
				margin-top:8px;
				border:1px solid #ccc;
				padding-left:5px;
				font-size:16px;
				font-family:raleway
			}
			input[type=submit] {
				width:100%;
				background-color:#FFBC00;
				color:#fff;
				border:2px solid #FFCB00;
				padding:10px;
				font-size:20px;
				cursor:pointer;
				border-radius:5px;
				margin-bottom:15px
			}
			p.a{
				color: red;

			}

		</style>
	</head>
	
	


	<body>
		<div id="main">
		<h1>Connexion</h1>
			<div id="login">
			<h2>Connexion</h2>
				<form action="login_post.php" method="post">
					<label>Username : </label>
					<input type="text" id="name" name="login" placeholder="Login">
					<br>
					<label>Password : </label>
					<input id="password" name="mdp" placeholder="********" type="password">
					<input name="submit" type="submit" value=" Se Connecter ">
					<span><?php echo $error; ?></span>
				</form>
				
			</div>
								<?php 
					if(isset($_GET['no'])){
						echo '<br><p class = "a">Erreur d\'authentification</p>';
					}
					?>
		</div>
		
		
		
		<?php
			include("footer.php");
		?>
		
	</body>
</html>
