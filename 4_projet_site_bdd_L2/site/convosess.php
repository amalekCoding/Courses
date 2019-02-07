<?php
session_start();


if (($type == 'correcteur') || ($type == 'surveillant') ) { 		/* On autorise que les candidats */
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

		<style type="text/css">
			body {
				background-color:#ffd;
				font-family:Verdana,Helvetica,Arial,sans-serif;
			}
			table { 
				margin:10px auto; border-collapse:collapse; border:1px solid gray; 
			}
			td, th {
				 border:1px solid gray; text-align:center; padding:20px;
			}
			h2 { border:10px solid gray; text-align:center; padding:20px; width: 300px;  margin-left: 480px;}

		</style>
    </head>
    
    <body>
    
		<h2>Convocation</h2>



		<section>
			<table>
				<tr><?php
					echo '<th colspan="4">'.$_GET['name_session'].'</th>';
					?>
				</tr>
				
				<tr>
				<th>Module</th> <th>Date/heure</th> <th>Salle</th> <th>Batiment</th>
				</tr>
				
				<tr>
					<form method= "POST" action="">
								
					<?php
					$resultats = $cnx->query('SELECT libelle, dateheure, nomsalle, nombatiment FROM epreuve ep, modulee m, salle s, batiment bat WHERE bat.idbatiment=s.batiment and ep.salle = s.idsalle and m.idmodule=ep.idmodule and ep.idsession = '.$_GET['idsession'].' ORDER BY codeepreuve');
					
					while($ligne = $resultats->fetch()){ 
						echo '<tr><td>' .$ligne['libelle'].'</td><td>' .$ligne['dateheure'].'</td><td>' .$ligne['nomsalle'].'</td><td>' .$ligne['nombatiment'].'</td>';
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



