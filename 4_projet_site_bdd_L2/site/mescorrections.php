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
							h3 { border:10px solid gray; text-align:center; padding:15px; width: 450px;  margin-left: 400px;}

		</style>
		
		<script type="text/javascript" src="aff_cacher_tab.js"></script>
    </head>
    
    <body><div id="boutonsidentite" class="boutonsp">
	<?php echo '<a href="liste_personne_note.php?idses='.$_GET['idses'].'"><-- RETOUR</a>' ?> 
	<button type="button" value="valider2" id="valider2" onmouseup="validerModif2();" disabled>Consulter mes corrections</button>
	<button type="button" value="creernouvellepers" id="creerpers">Voir resultats --></button>
	<br/><br/>
</div>
    
		<h3>Mes corrections : </h3>

			<form method= "GET" action="teston.php">

			  <fieldset>
				<legend>Je peux corriger : <em>*</em></legend>


				<?php
					$resultats = $cnx->query('SELECT DISTINCT m.libelle, m.idmodule, m.libelle FROM peut_corriger pc JOIN modulee m ON m.idmodule=pc.idmodule JOIN contient c ON c.idmodule = m.idmodule WHERE c.idsession='.$_GET['idses'].' AND pc.idcorrecteur ='.$_SESSION['numpersonne']);
					foreach($resultats  as $key => $value)
					{  						

						echo '<a href="mescorrections.php?id_module='.$value['idmodule'].'&amp;idses='.$_GET['idses'].'">'.$value['libelle'].'</a><br />';
						echo '<br />';

					}	
					
					$req = $cnx->query('SELECT nom FROM session WHERE idsession = '.$_GET['idses']);
					$nameses = $req->fetch();
					
					if(isset($_GET['id_module'])){
						$idmodule = $_GET['id_module'];
						$req = $cnx->query('SELECT libelle FROM modulee WHERE idmodule = '.$idmodule);
						$nommodul = $req->fetch();
					}
					else{ 
						$idmodule = 0;
					}
?>
			</fieldset>
			
			
			


		<section>
			
			<table>
				
				

				<tr>
					<th colspan="20">
						<?php
							echo $nameses['nom']. ' : <br> ' .$nommodul['libelle'];
						?>
					</th>
				</tr>

				<tr>
					<th colspan="7">Nom</th>
					<th colspan="7">Prenom</th>
					<th colspan="5">Note /20</th>

				</tr>
				
				
				<tr>			
				  <?php
				  
				  
					$numpersonne = $_SESSION['numpersonne'];
				
				  
					$idses = $_GET['idses'];
					$id_module = $_GET['id_module'];
					
					if(!isset($_GET['id_module'])){
						echo '<tr><td colspan="15">Veuiller selectionez un module</td></tr>';
						}
				  

				  	$res = $cnx->query("SELECT codeepreuve FROM epreuve WHERE idsession =".$idses." and idmodule =".$id_module);
if($res){
					$idepreuve = $res->fetch();
					


					$resultats = $cnx->query("SELECT * FROM corrige c JOIN personne p ON p.numpersonne = c.numpersonne WHERE codeepreuve =".$idepreuve[0]." and idcorrecteur =".$_SESSION['numpersonne']);
			
					$cpt_person = 0;
					while($ligne = $resultats->fetch()){
						$cpt_person  = $cpt_person + 1;

						echo '<tr>';
						echo '<td colspan="10">'.$ligne['nom'].'</td><td colspan="3">'.$ligne['prenom'].'</td>';
												
						echo '<td  colspan="5">'.$ligne['note'].'/20</td>';

						?>
						</tr>

						

				<?php }}
					
					  if($cpt_person == 0){
					  	 echo '<tr><td colspan="15">Personne que vous avez corriger</td></tr>';
					  }
			      ?>
				</tr>

				
			</table>
			
		</section>

		 
		<section>
			<table>
				<th colspan="5">Note  mini : </th>	
				<tr>
					<td>
						<?php
						$resultats = $cnx->query("SELECT note_min FROM contient WHERE idsession =".$idses." and idmodule =".$id_module);
						if($resultats){
						$idse = $resultats->fetch();

						echo $idse['note_min'].'/20';
					}
						?>
					</td>
				</tr>	

			</table>
		</section>
		
		
		<?php
		if(!empty($idses) && !empty($id_module)){
			echo "<input name = 'session' value =".$idses." type= 'hidden'/>";
			echo "<input name = 'mod' value =".$id_module." type= 'hidden'/>";
				}
		?>


		</section>

		</form>
		
		<?php
			include("footer.php");
		?>
		
	</body>
</html>




