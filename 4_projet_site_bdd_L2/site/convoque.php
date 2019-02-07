<?php
session_start();


$type = $_SESSION['type'];
$type=preg_replace('/\s/', '', $type);

if (($type == 'correcteur') || ($type == 'surveillant') ) { 						/* On autorise que les candidats */
	header('Location: index.php'); 
	die();
}
else if(!$type){
	echo "<script type='text/javascript'>document.location.replace('login.php')</script>";	
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

		<style type="text/css">
			body {
				background-color:#ffd;
			}
			table { margin:10px auto; border-collapse:collapse; border:1px solid gray; }
			td,th { border:3px solid gray; text-align:center; padding:20px; width: 500px;}
			h2 { border:10px solid gray; text-align:center; padding:20px; width: 300px;  margin-left: 460px;}

		</style>
    </head>
    
    <body>
    
		


		
		<section>
			<table>
				
				<h2>Vos convocation</h2>
				<tr>
							<?php
							$res = $cnx->query("SELECT numpersonne FROM personne WHERE login = '".$_SESSION['login']."'");
							$idpersonne = $res->fetch();

							$resultats = $cnx->query('SELECT idsession, nom FROM inscrit i, session s WHERE i.codepersonne = '.$idpersonne[0].' and i.codesession = s.idsession ORDER BY datedeb DESC');

							while($ligne = $resultats->fetch()){ 
								echo '<tr><td><a href="convosess.php?idsession='.$ligne['idsession'].'&amp;name_session='.$ligne['nom'].'">'.$ligne['nom'].'</a></td></tr>';

							}
							?>
					
					

				</tr>
			</table>
		</section>

		
		<?php 
			/* On verifie que la personne n'a pas validé 10 modules donc l'examen et peut se réinscrire. ----- */ 
		$resultats = $cnx->query('SELECT count(*) FROM valide_module WHERE numpersonne = '.$idpersonne[0].' GROUP BY numpersonne, idsession HAVING count(*)=10');
		$valide = $resultats->fetch();
		
		if(empty($valide[0])){
		?>
			<section>
				<table>
					<tr><td>
								S'inscrire a une nouvelle session ?

							<form method= "POST" action="#">
							   <select name="session">
								<?php
								$resultats = $cnx->query('SELECT * FROM session WHERE datedeb > date(now()) AND idsession NOT IN (SELECT codesession FROM inscrit WHERE codepersonne = '.$idpersonne[0].')  ORDER BY datedeb DESC');

								while($ligne = $resultats->fetch()){ 
									echo ' <option value='.$ligne['idsession'].'>'.$ligne['nom'].'</option>';

								}
							
							
								?>
							</select>
							<input type="submit" name="Ajouter" value="Ajouter"/><br/>
						</form>
					</td></tr>
				</table>
		</section>
 	<?php }  
		else {

		?><section>
				<table>
					<tr><td>
						BRAVO ! <br> VOUS AVEZ REUSSI VOTRE EXAMEN

					</td></tr>
				</table>
		</section>

	<?php	}   ?>






		<?php
    	if(isset($_POST["session"])){
			$resultats = $cnx->query('INSERT INTO inscrit VALUES('.$_POST["session"].','.$idpersonne[0].')');
			echo "<script type='text/javascript'>document.location.replace('convoque.php');
				</script>";
		}
		?>
		
		
		<?php
			include("footer.php");
		?>
		
	</body>
</html>



