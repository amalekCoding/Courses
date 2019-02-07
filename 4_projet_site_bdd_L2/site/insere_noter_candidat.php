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
        <title>Choix Module</title>
        <link rel="stylesheet" href="style.css">

		<style type="text/css">
			body {
				background-color:#ffd;
				font-family:Verdana,Helvetica,Arial,sans-serif;
			}
			table {  padding: 10px; margin-left: 250px; float : left;
	  
					border-collapse:collapse; border:1px solid gray; }
			td,th { border:1px solid gray; text-align:center; }
			
			table
			{
				border-collapse : collapse;
				background-color : #EBD9C8;
			}

			th,td
			{
				border : 1px solid black;
				height : 40px;
				
				float : center;
			}

			th
			{
				background-color : #D48030;
			}

			.impair
			{
				background-color : #BEA58E;
			}

				fieldset {
				  margin-bottom: 15px;
				  padding: 10px;
				  margin-left: 80px;
				  width: 200px;
				  float : left;
				}
		</style>
    </head>
    
    <body>			
			
			<?php
			      
			     if(!isset($_POST['mod']) ){
					echo "<script type='text/javascript'>document.location.replace('index.php');	
							 </script>";				 
				 } 
				 
				 
				$session = $_POST['session'];
				$id_module = $_POST['mod'];
				if((!empty($_POST['notemin'])) && ($_POST['notemini'] == "Modifier")){
					$note_mini = $_POST['notemin'];
					$res = $cnx->query("UPDATE contient SET note_min =".$note_mini." WHERE idsession =".$session." and idmodule =".$id_module);
					echo "<script type='text/javascript'>document.location.replace('liste_personne_note.php?idses=".$session."&id_module=".$id_module."');	
						 </script>";
				
				}
				else if(($_POST['notemini'] == "Modifier") && empty($_POST['notemin'])){
					$res = $cnx->query("UPDATE contient SET note_min = NULL WHERE idsession =".$session." and idmodule =".$id_module);
					echo "<script type='text/javascript'>document.location.replace('liste_personne_note.php?idses=".$session."&id_module=".$id_module."');	
						 </script>";
				}
				
				$res = $cnx->query("SELECT codeepreuve FROM epreuve WHERE idsession =".$session." and idmodule =".$id_module);

				$idepreuve = $res->fetch();
				
			
				$resultats = $cnx->query("SELECT * FROM a".$session." WHERE NOT EXISTS (SELECT * FROM corrige c WHERE codeepreuve = ".$idepreuve[0]." and c.numpersonne =a".$session.".numpersonne)");
				
				
				
				
				
				
						
				$cpt = 0;
				
				
				
				
				while($ligne = $resultats->fetch()){ 
					echo '<tr>';
										
					if($_POST['Note'][$cpt]!= NULL){
						echo 'note = '.$_POST['Note'][$cpt];
						echo 'num = '.$ligne[0];
						echo '<td colspan="10"> '.$ligne['nom'].'</td><td colspan="3">'.$ligne['prenom'].'</td><br>';
					echo '<br>';
					$cnx->query('INSERT INTO corrige VALUES('.$_SESSION['numpersonne'].','.$idepreuve[0].','.$ligne[0].','.$_POST['Note'][$cpt].')');
					}
					echo '</tr>';
					$cpt = $cpt + 1;
					
				}
		
				?>
					

		<?php
		
			
				$req = $cnx->query('SELECT co.idsession, co.idmodule, c.numpersonne, note, note_min FROM contient co, corrige c, epreuve e WHERE co.idsession=e.idsession AND co.idmodule=e.idmodule AND c.codeepreuve = e.codeepreuve  AND co.idsession = '.$session.' AND co.idmodule = '.$id_module.' AND note >= note_min');
				while($pers_valid_mod = $req->fetch()){	
					$resultats = $cnx->query('INSERT INTO valide_module VALUES('.$pers_valid_mod['numpersonne'].','.$session.','.$id_module.')');
				}






								/* 				<----- Redirection 			*/
				echo "<script type='text/javascript'>document.location.replace('liste_personne_note.php?idses=".$session."&id_module=".$id_module."');	
						 </script>";		
		?>
		
		

						 
	
		
	</body>
</html>






