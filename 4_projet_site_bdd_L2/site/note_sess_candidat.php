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
						td,th { border:4px solid gray; text-align:center; padding:20px; width: 100px;}
			h2 { border:10px solid gray; text-align:center; padding:20px; width: 300px;  margin-left: 480px;}

		</style>
    </head>
    
    <body>
    
		<h2>Resultats :</h2>



		<section>
			<table>
				<tr>
					
					<tr><th>Epreuve </th><th>Note Candidat </th><th>Note Min </th><th>Note Max</th><th>Moyenne epreuve</th></tr>

					
					<tr>
					
				<?php
					$req = $cnx->query(' SELECT * FROM corrige c,  modulee m, epreuve e LEFT OUTER JOIN contient con ON con.idsession = e.idsession and con.idmodule = e.idmodule  LEFT OUTER JOIN (SELECT e.codeepreuve, round(AVG(note),2) as moy_ep FROM corrige c, modulee m, epreuve e  WHERE m.idmodule=e.idmodule and e.codeepreuve=c.codeepreuve and e.idsession= '.$_GET['idses'].' GROUP BY e.codeepreuve) as moy_e ON moy_e.codeepreuve = e.codeepreuve LEFT OUTER JOIN (SELECT e.codeepreuve, min(note), max(note) FROM corrige c, modulee m, epreuve e  WHERE m.idmodule=e.idmodule and e.codeepreuve=c.codeepreuve and e.idsession= '.$_GET['idses'].' GROUP BY e.codeepreuve) as min_max ON min_max.codeepreuve= e.codeepreuve WHERE m.idmodule=e.idmodule and e.codeepreuve=c.codeepreuve and e.idsession= '.$_GET['idses'].' and c.numpersonne='.$_GET['idpers']);
					while($ligne = $req->fetch()){ 
						
						if($ligne['note'] < 10){
							echo '<tr><td>'.$ligne['libelle'].'</td><th>0'.$ligne['note'].' / 20</th>';
						}
						else 
							echo '<tr><td>'.$ligne['libelle'].'</td><th> '.$ligne['note'].' / 20</th>';



						if($ligne['min'] < 10){
							echo '<td>0'.$ligne['min'].' / 20</td>';
						}
						else 
							echo '<td> '.$ligne['min'].' / 20</td>';


						if($ligne['max'] < 10){
							echo '<td>0'.$ligne['max'].' / 20</td>';
						}
						else 
							echo '<td> '.$ligne['max'].' / 20</td>';					
				
				
						if($ligne['moy_ep'] < 10){
							echo '<td>0'.$ligne['moy_ep'].' / 20</td>';
						}
						else 
							echo '<td> '.$ligne['moy_ep'].' / 20</td>';

					
					
					echo  '</tr>';
			}		
			?>
				</tr>
				

					
					</td>

				</tr>
			</table>
		</section>
		
		

		
				<section>
					<table>
						<?php			
						
							$res = $cnx->query('SELECT round(AVG(note),2) FROM epreuve e, corrige c WHERE c.codeepreuve= e.codeepreuve and e.idsession = '.$_GET['idses'].' and c.numpersonne = '.$_GET['idpers']);
							$moy_cand = $res->fetch();
							
							echo '<tr><td>Moyenne : </td><th> '.$moy_cand[0].' /20</th>';

							$requ = $cnx->query('SELECT round(AVG(min),2) as moy_min, round(AVG(max),2) as moy_max FROM (SELECT e.codeepreuve, min(note), max(note) FROM epreuve e, corrige c WHERE c.codeepreuve= e.codeepreuve and e.idsession = '.$_GET['idses'].' GROUP BY e.codeepreuve) as moy_clas');
							$moy_clas = $requ->fetch();

							echo '<td>'.$moy_clas['moy_min'].' /20</td><td> '.$moy_clas['moy_max'].' /20</td>';

							$reque = $cnx->query('SELECT round(AVG(moy_ep),2) as moy_epr FROM (SELECT e.codeepreuve, AVG(note) as moy_ep FROM epreuve e, corrige c WHERE c.codeepreuve= e.codeepreuve and e.idsession = '.$_GET['idses'].' GROUP BY e.codeepreuve) as moy_ep');
							$moy_ep = $reque->fetch();

							echo '<td>'.$moy_ep['moy_epr'].' /20</td>'
							?>
		
					</table>
		</section>
	


	<?php
		$requet = $cnx->query('SELECT numpersonne, idsession, count(note) FROM inscrit i, corrige c, epreuve e WHERE i.codesession = e.idsession AND i.codepersonne = c.numpersonne AND c.codeepreuve=e.codeepreuve AND i.codepersonne = '.$_GET['idpers'].' AND i.codesession = '.$_GET['idses'].' GROUP BY numpersonne, idsession HAVING count(note)=10;');
		$en_cours = $requet->fetch();

		if(empty($en_cours)){ 
			
?>
			<section>
					<table>
									

							
							<tr><th>En cours de correction</th></tr>

						
		
					</table>
		</section>


		<?php } 
		else {
			
			
			
			$requete = $cnx->query('SELECT count(*) FROM contient co, corrige c, epreuve e WHERE co.idsession=e.idsession AND co.idmodule=e.idmodule AND c.codeepreuve = e.codeepreuve  AND co.idsession = '.$_GET['idses'].' AND numpersonne = '.$_GET['idpers'].' AND note >= note_min'); 
			$nb = $requete->fetch();
			if($nb[0] == 10){
				?>
				<section>
						<table>						
								<tr><td>Resultats : </td><th>Admis</th></tr>
			
						</table>
				</section>	
			<?php 
			}
			else{
				?>
				<section>
						<table>						
								<tr><td>Resultats : </td><th>Non Admis</th></tr>
			
						</table>
				</section>	
			<?php 
			}
		}
			?>
		
		<?php
			include("footer.php");
		?>
		
	</body>
</html>



