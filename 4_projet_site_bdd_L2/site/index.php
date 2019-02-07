<?php
session_start();
include("menu.php");
?>

	</body>
</html>
<?php
		if(isset($_SESSION['type'])){
			if($_SESSION['type']== 'correcteur'){
				echo "<script type='text/javascript'>document.location.replace('corrige.php')</script>";	
			}	
			else{
				echo "<script type='text/javascript'>document.location.replace('convoque.php')</script>";	

			}
		}
		else{
			echo "<script type='text/javascript'>document.location.replace('login.php')</script>";	}


include("footer.php");
?>

   </body>
</html>
