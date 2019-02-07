<?php
session_start();
include("menu.php");
include("connexion_bd.php");
?>

<!DOCTYPE html>
<html>
    <head>
		
        <meta charset="utf-8">
        <title>Convocation</title>
        <link rel="stylesheet" href="style.css">

		<style type="text/css">
			body {
				background-color:#ffd;
			}
			table { margin:10px auto; border-collapse:collapse; border:1px solid gray; }
			td,th, h2 { border:1px solid gray; text-align:left; padding:20px; }
		</style>
    </head>
    
    <body>
    
		<h2>S'inscrire a une session</h2>


		<section>
			<table>
				<tr>
					<form method= "POST" action="#">

							<?php
							
							$res = $cnx->query("SELECT numpersonne FROM personne WHERE login = '".$_SESSION['login']."'");
							$numpersonne = $res->fetch();
							$resultats = $cnx->query('SELECT nom FROM inscrit i, session s WHERE i.codepersonne = '.$numpersonne.' and i.codesession = s.idsession');

							while($ligne = $resultats->fetch()){ 
								echo '<a href="convosess.php?name_session='.$ligne['nom'].'">'.$ligne['nom'].'</a>';

							}
							?>
					
					</form>

				</tr>
			</table>
		</section>
		
		
		<?php
			include("footer.php");
		?>
		
	</body>
</html>



