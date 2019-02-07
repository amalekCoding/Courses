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
				font-family:Verdana,Helvetica,Arial,sans-serif;
			}
			table { margin:10px auto; border-collapse:collapse; border:1px solid gray; }
			td,th, h2 { border:1px solid gray; text-align:center; padding:20px; }
		</style>
    </head>
    
    <body>
    
		<h2>Convocation</h2>



		<section>
			<table>
				<tr><?php
					echo '<th colspan="4">'.$_GET['name_session'].'</th>';
					
					
					
					
					

					$numpersonne = $_SESSION['numpersonne'];
					
					
					
					$resultats = $cnx->query('SELECT codesession FROM inscrit WHERE codepersonne = '.$numpersonne);
					
					foreach($resultats  as $key => $value)
					{            
						echo $key. "=> ".$value['codesession']."<br/>\n";
					
					
						$res = $cnx->query('SELECT libelle FROM contient c, modulee m WHERE c.idmodule = m.idmodule AND c.idsession ='.$value['codesession'].'');
						foreach($res  as $cle => $valu){
							echo $cle. "=> ".$valu['libelle']."<br/>\n";

						}
					}
					
					
						$resultats = $cnx->query('SELECT m.libelle, e.dateheure, s.nomsalle, b.nombatiment FROM salle s, modulee m, convoque c, epreuve e, batiment b WHERE b.idbatiment = s.batiment and c.numpersonne = 1 and e.codeepreuve = c.codeepreuve and m.idmodule= e.idmodule and s.idsalle=c.idsalle');
							echo '<tr><th>Module</th><th>Dateheure</th><th>Salle</td><th>Batiment</th></tr>';
						while($ligne = $resultats->fetch()){ 
							echo '<tr><td>'.$ligne['libelle'].'</td><td>' .$ligne['dateheure'].'</td><td>'.$ligne['nomsalle'].'</td><td>'.$ligne['nombatiment'].'</td></tr>';

						
						
						
						}

					?>
				</tr>
				<tr>
					<form method= "POST" action="">
								
							<?php
							$resultats = $cnx->query('SELECT m.libelle, e.dateheure, s.nomsalle, b.nombatiment FROM salle s, modulee m, convoque c, epreuve e, batiment b WHERE b.idbatiment = s.batiment and c.numpersonne = 1 and e.codeepreuve = c.codeepreuve and m.idmodule= e.idmodule and s.idsalle=c.idsalle');
								echo '<tr><th>Module</th><th>Dateheure</th><th>Salle</td><th>Batiment</th></tr>';
							while($ligne = $resultats->fetch()){ 
								echo '<tr><td>'.$ligne['libelle'].'</td><td>' .$ligne['dateheure'].'</td><td>'.$ligne['nomsalle'].'</td><td>'.$ligne['nombatiment'].'</td></tr>';

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



