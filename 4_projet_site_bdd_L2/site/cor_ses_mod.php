<?php
session_start();

if (!isset($_SESSION['login']) || $_SESSION['type'] != 'correcteur'){									
	header('Location: index.php'); 					/* On autorise que les correcteurs */
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
			table { margin:10px auto; border-collapse:collapse; border:1px solid gray; }
			td,th, h2 { border:1px solid gray; text-align:center; padding:20px; }
		</style>
    </head>
    
    <body>
    
		<h2>Corriger</h2>



		<section>
			<table>
				<tr><?php
					echo '<th colspan="4">'.$_GET['name_session'].'</th>';
					
					

					$numpersonne = $_SESSION['numpersonne'];
					
					$rese = $cnx->query("SELECT idsession FROM session WHERE nom = '".$_GET['name_session']."'");

					$idses = $rese->fetch();

					
					

					$resultats = $cnx->query('SELECT DISTINCT m.libelle, m.idmodule, m.libelle FROM peut_corriger pc JOIN modulee m ON m.idmodule=pc.idmodule JOIN contient c ON c.idmodule = m.idmodule WHERE c.idsession='.$idses[0].' AND pc.idcorrecteur ='.$numpersonne);
					foreach($resultats  as $key => $value)
					{  						
						
						
						echo '<tr><td><a href="liste_personne_note.php?nom_mod='.$value['libelle'].'&amp;id_module='.$value['idmodule'].'&amp;name_ses= '.$_GET['name_session'].'&amp;idses='.$idses[0].'">'.$value['libelle'].'</a></tr></td>';
					

					}
					
					

					?>
				</tr>


		
			</table>
		</section>
		
		
		<?php
			include("footer.php");
		?>
		
	</body>
</html>



