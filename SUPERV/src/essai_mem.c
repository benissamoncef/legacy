/* Essai de locker un process en memoire */

#include <sys/resource.h>
#include <sys/lock.h>
#include <errno.h>


int main(argc, argv)
 int argc ;
 char **argv ;
{

#define         C_MEM    "TAILLE_MEM_TEST"

char *  Recup_Taille_Mem     = NULL;
int     Valeur_Taille_Mem    = 0;
char 	* vl_Tab_Char	= NULL;
int 	code_retour = 0;


	/* Recuperation de lal taille en memoire */
	Recup_Taille_Mem = getenv(C_MEM);
	Valeur_Taille_Mem = atoi(Recup_Taille_Mem);

	printf (" Valeur de la memoire a locker %d \n",Valeur_Taille_Mem);

	/* Allocation memoire */
	vl_Tab_Char = (char *) malloc( (Valeur_Taille_Mem) * sizeof(char) );

	/* Mise en memoire */
	errno = 0;
	if ((code_retour = plock (PROCLOCK))== -1)
	{
		/*Erreur detectee pour locker la totalite de tdpac en memoire*/
		perror("Impossible de locker la totalite de la tache en memoire vive :");
		printf("plock : Impossible de locker la totalite de la tache en memoire vive : code de retour : %d",
								code_retour);
	}

	/* Boucle Infinie */
	for (;;)
	{
		sleep(10);
	}

}
