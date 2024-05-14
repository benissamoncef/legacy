/*********************************************************
*	Fichier permettant de tester le bon fonctionnement
*	des modulesd 'interfaces XZSA 
*
***********************************************************/

#include <stdio.h>
#include <rtworks/common.h>
#include <rtworks/ipc.h>
#include <varargs.h>

#include "xdc.h"
#include "xzsm.h"
#include "xzst.h"
#include "xzsa.h"


#define REPERTOIRE      "/users/milleville/migrazur/SUPERV/tests"
#define Nom_Fic_Result  REPERTOIRE"/tests_result.xzsa"
#define Mode_Write      "w"

FILE * 	FIC_DIAL;	/* Fichier de dialogue */
FILE *  Fic_Result;	/* Fichier de resultat */

/* declaration de fonctions internes */
typedef char *myVaListe;

#define myVaStart(liste, paramN) va_start(liste, paramN)
#define myVaArg(liste, mode)     va_arg(liste, mode)
#define myVaEnd(liste) liste=NULL

int 	vg_Test20 = 0;




/***************
*       Fonction permettant d'ouvrir un fichier de resultat
*
**************/
FILE * open_fic_result(char * nom_fichier)
{
        FILE *f;


        f = fopen(nom_fichier,Mode_Write);
        if (f == NULL)
        {
                printf (" Probleme sur ouverture fichier resultat %s "
                , nom_fichier);
        }
        return f;


}       /* fin open_fic_result */



/***************
*       Fonction permettant de fermer un fichier
*
**************/
void close_fichier(FILE * fichier)
{

        /* fermeture fichier */
        fclose(fichier);

}       /* fin close_fichier */




/**** Fonction Affiche Resultat ***/
void Affiche_Result(int	va_NumTest, ... )
                       
{
	va_list		pl_listeArgument;
	char *		vl_Arg[10]; 
	int		vl_NbArg 	= 0;
	int		vl_Resultat 	= XDC_OK;
	char 		vl_Donnees[80] = "\0";
	int		i;


	/* Recuperation des arguments */
        myVaStart (pl_listeArgument, va_NumTest);

        while ( (vl_Arg[vl_NbArg++] = myVaArg(pl_listeArgument, char*)) 
								!= NULL);
									
									

        /* Liberation du pointeur sur argument*/
        myVaEnd (pl_listeArgument);


	for (i=0; i<(vl_NbArg-1); i=i+2)
	{
		/* Test si valeur attendue = valeur recue */
		if (strcmp(vl_Arg[i], vl_Arg[i+1]))
			vl_Resultat = XDC_NOK;

		/*  Construction de la chaine de caracteres **/
		sprintf(vl_Donnees,"%s -- %s", vl_Donnees, vl_Arg[i]);
	}

	if (vl_Resultat == XDC_OK)
	{
                fprintf (Fic_Result,
                        "XZSA_%02u  (Donnee Recue  %s)\
                                \n\t\t--> OK \n",va_NumTest, vl_Donnees);

        }
        else
                fprintf (Fic_Result,
                        "XZSA_%02u  (Donnee Recue  %s)\
                                \n\t\t--> NOK \n",va_NumTest, vl_Donnees);


}       /* Affiche_Result**/

void test_xzsa20(int Num_Test, int flag)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];

if (flag == XDC_VRAI)
{
	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_20.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSA_20InitAdmin();

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );
	}
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_20.. avec XZSAV_InitOk = XDC_VRAI
		*/
		vl_ValRet = XZSA_20InitAdmin();

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );

		/* Affectation Test20**/
		vg_Test20 = 1;

}
else
{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_20.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSA_20InitAdmin();

		/* Affectation Test20**/
		vg_Test20 = 1;
}


}	/* fin test_xzsa20 */

void test_xzsa01(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	int			vl_NbMach		= 0;
	int			vl_NbMach_Expected	= 9999;
	char 			vl_Str_NbMach[10];
	char 			vl_Str_NbMach_Ex[10];


	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_01.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSA_01ListeMachine( NULL, &vl_NbMach,NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_01.. 
				avec XZSAV_InitOk = XDC_VRAI
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_01ListeMachine( NULL, &vl_NbMach,NULL);
		if (vl_NbMach != 0)
			vl_NbMach_Expected = vl_NbMach;
		else
			vl_NbMach_Expected = 9999;

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		sprintf(vl_Str_NbMach, "%d", vl_NbMach);
		sprintf(vl_Str_NbMach_Ex, "%d", vl_NbMach_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, vl_Str_NbMach,
				vl_Str_NbMach_Ex, NULL  );

}	/* fin test_xzsa01 */



void test_xzsa02(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomMachine 	vl_NomMach;
	int			vl_NbSSyst		= 0;
	int			vl_NbSSyst_Expected	= 9999;
	char 			vl_Str_NbSSyst[10];
	char 			vl_Str_NbSSyst_Ex[10];

	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_02.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_02ListeSSysteme(vl_NomMach,
				NULL, &vl_NbSSyst,NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_02.. 
				avec XZSAV_InitOk = XDC_VRAI
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_02ListeSSysteme(vl_NomMach,
				NULL, &vl_NbSSyst,NULL);
		if (vl_NbSSyst != 0)
			vl_NbSSyst_Expected = vl_NbSSyst;
		else
			vl_NbSSyst_Expected = 9999;

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		sprintf(vl_Str_NbSSyst, "%d", vl_NbSSyst);
		sprintf(vl_Str_NbSSyst_Ex, "%d", vl_NbSSyst_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, vl_Str_NbSSyst,
				vl_Str_NbSSyst_Ex, NULL  );

}	/* fin test_xzsa02 */


void test_xzsa03(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomMachine 	vl_NomMach;
	int			vl_NbMach		= 0;
	int			vl_NbMach_Expected	= 9999;
	char 			vl_Str_NbMach[10];
	char 			vl_Str_NbMach_Ex[10];

	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_03.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_03ListeTacheMachine(vl_NomMach,
				NULL, &vl_NbMach,NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_03.. 
				avec XZSAV_InitOk = XDC_VRAI
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_03ListeTacheMachine(vl_NomMach,
				NULL, &vl_NbMach,NULL);
		if (vl_NbMach != 0)
			vl_NbMach_Expected = vl_NbMach;
		else
			vl_NbMach_Expected = 9999;

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		sprintf(vl_Str_NbMach, "%d", vl_NbMach);
		sprintf(vl_Str_NbMach_Ex, "%d", vl_NbMach_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, vl_Str_NbMach,
				vl_Str_NbMach_Ex, NULL  );


}	/* fin test_xzsa03 */

void test_xzsa04(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomMachine 	vl_NomMach;
	int			vl_NbSSyst		= 0;
	int			vl_NbSSyst_Expected	= 9999;
	char 			vl_Str_NbSSyst[10];
	char 			vl_Str_NbSSyst_Ex[10];
	XZSCT_NomSSysteme       vl_NomSSysteme = "SUPERV";



	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_02.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_04ListeTacheMachineSSyst(
			vl_NomMach,vl_NomSSysteme,
				NULL, &vl_NbSSyst,NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_02.. 
				avec XZSAV_InitOk = XDC_VRAI
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_04ListeTacheMachineSSyst(
			vl_NomMach,vl_NomSSysteme,
				NULL, &vl_NbSSyst,NULL);
		if (vl_NbSSyst != 0)
			vl_NbSSyst_Expected = vl_NbSSyst;
		else
			vl_NbSSyst_Expected = 9999;

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		sprintf(vl_Str_NbSSyst, "%d", vl_NbSSyst);
		sprintf(vl_Str_NbSSyst_Ex, "%d", vl_NbSSyst_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, vl_Str_NbSSyst,
				vl_Str_NbSSyst_Ex, NULL  );

}	/* fin test_xzsa04 */




void test_xzsa05(int Num_Test)
{

	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomMachine 	vl_NomMach;


	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_05.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_05ExisterMachine(vl_NomMach);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_05.. 
				avec XZSAV_InitOk = XDC_VRAI
		* et Nom Machine Connu
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_05ExisterMachine(vl_NomMach);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL  );

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_01.. 
				avec XZSAV_InitOk = XDC_VRAI
		* et Nom Machine Connu
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_05ExisterMachine(NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_NOM_INV;
		
		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL  );


}	/* fin test_xzsa05 */


void test_xzsa06(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomSSysteme       vl_NomSSysteme 		= "SUPERV";

	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_06.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSA_06ExisterSSysteme(vl_NomSSysteme);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_06.. 
				avec XZSAV_InitOk = XDC_VRAI
		*  et sous systeme connu 
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_06ExisterSSysteme(vl_NomSSysteme);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL  );

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_06.. 
				avec XZSAV_InitOk = XDC_VRAI
		*  et sous systeme inconnu 
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_06ExisterSSysteme(NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_NOM_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL  );

}	/* fin test_xzsa06 */





void test_xzsa07(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomTache          vl_NomTache		= "tshor.x";


	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_07.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSA_07ExisterTache(vl_NomTache);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_07.. 
				avec XZSAV_InitOk = XDC_VRAI
		*  et Tache connu 
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_07ExisterTache(vl_NomTache);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL  );

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_07.. 
				avec XZSAV_InitOk = XDC_VRAI
		*  et Tache inconnu 
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_07ExisterTache(NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_NOT_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL  );


}	/* fin test_xzsa07 */




void test_xzsa08(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomTache          vl_NomTache		= "tshor.x";
	XZSCT_NomSSysteme       vl_NomSSysteme;
	XZSCT_NomSSysteme       vl_NomSSysteme_Ex	= "SUPERV";

	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_08.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSA_08TacheNomSSysteme(
					vl_NomTache, vl_NomSSysteme);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_08.. 
				avec XZSAV_InitOk = XDC_VRAI
		*  et Sous System connue 
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_08TacheNomSSysteme(
					vl_NomTache, vl_NomSSysteme);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, vl_NomSSysteme,
				vl_NomSSysteme_Ex, NULL  );

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_08.. 
				avec XZSAV_InitOk = XDC_VRAI
		*  et Sous Systeme inconnue 
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_08TacheNomSSysteme(
					NULL, vl_NomSSysteme);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_NOT_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL );



}	/* fin test_xzsa08 */


void test_xzsa11(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomTache		vl_NomTache		= "tshor.x";
	XZSCT_NomTache		vl_NomTache_Tmp		= "terau.x";


	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_11.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSA_11TacheTemporaire( vl_NomTache );

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_11.. 
				avec XZSAV_InitOk = XDC_VRAI
		*  et Tache  connue 
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_11TacheTemporaire( vl_NomTache_Tmp );

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL );

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_11.. 
				avec XZSAV_InitOk = XDC_VRAI
		*  et Tache  inconnue 
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_11TacheTemporaire( NULL );

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_NOT_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL );

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_11.. 
				avec XZSAV_InitOk = XDC_VRAI
		*  et Tache  connue  mais Permanente
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_11TacheTemporaire( vl_NomTache );

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_TYPE_TPER;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL );


}	/* fin test_xzsa11 */


void test_xzsa12(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomMachine 	vl_NomMach;
	int			vl_NbMach		= 0;
	int			vl_NbMach_Expected	= 9999;
	char 			vl_Str_NbMach[10];
	char 			vl_Str_NbMach_Ex[10];

	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_12.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_12ListeTacheTemporaire(vl_NomMach,
				NULL, &vl_NbMach,NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_12.. 
				avec XZSAV_InitOk = XDC_VRAI
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_12ListeTacheTemporaire(vl_NomMach,
				NULL, &vl_NbMach,NULL);
		if (vl_NbMach != 0)
			vl_NbMach_Expected = vl_NbMach;
		else
			vl_NbMach_Expected = 9999;

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		sprintf(vl_Str_NbMach, "%d", vl_NbMach);
		sprintf(vl_Str_NbMach_Ex, "%d", vl_NbMach_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, vl_Str_NbMach,
				vl_Str_NbMach_Ex, NULL  );

}	/* fin test_xzsa12 */


void test_xzsa13(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomMachine 	vl_NomMach;
	int			vl_NbMach		= 0;
	int			vl_NbMach_Expected	= 9999;
	char 			vl_Str_NbMach[10];
	char 			vl_Str_NbMach_Ex[10];

	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_13.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_13ListeTachePermanente(vl_NomMach,
				NULL, &vl_NbMach,NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_12.. 
				avec XZSAV_InitOk = XDC_VRAI
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_13ListeTachePermanente(vl_NomMach,
				NULL, &vl_NbMach,NULL);
		if (vl_NbMach != 0)
			vl_NbMach_Expected = vl_NbMach;
		else
			vl_NbMach_Expected = 9999;

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		sprintf(vl_Str_NbMach, "%d", vl_NbMach);
		sprintf(vl_Str_NbMach_Ex, "%d", vl_NbMach_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, vl_Str_NbMach,
				vl_Str_NbMach_Ex, NULL  );

}	/* fin test_xzsa13 */




void test_xzsa17(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomMachine 	vl_NomMach;
	int			vl_TypeMach = 0;
	int			vl_TypeMach_Expected = 9999;
	char 			vl_Str_TypeMach[10];
	char 			vl_Str_TypeMach_Ex[10];


	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_17.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_17TypeMachine(vl_NomMach, &vl_TypeMach);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_17.. 
				avec XZSAV_InitOk = XDC_VRAI
		* et Nom Machine Connu
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_17TypeMachine(vl_NomMach, &vl_TypeMach);

		if (vl_TypeMach != 0)
			vl_TypeMach_Expected = vl_TypeMach;
		else
			vl_TypeMach_Expected = 9999;
	
		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		sprintf(vl_Str_TypeMach, "%d", vl_TypeMach);
		sprintf(vl_Str_TypeMach_Ex, "%d", vl_TypeMach_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, 
				vl_Str_TypeMach, vl_Str_TypeMach_Ex, NULL  );

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_17.. 
				avec XZSAV_InitOk = XDC_VRAI
		* et Nom Machine Connu
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_17TypeMachine(NULL, &vl_TypeMach);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_NOM_INV;
		
		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL  );



}	/* fin test_xzsa17 */


void test_xzsa18(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	int			vl_NbMach		= 0;
	int			vl_NbMach_Expected	= 9999;
	char 			vl_Str_NbMach[10];
	char 			vl_Str_NbMach_Ex[10];


	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_18.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSA_18ListeSite( NULL, &vl_NbMach,NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_18.. 
				avec XZSAV_InitOk = XDC_VRAI
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_18ListeSite( NULL, &vl_NbMach,NULL);
		if (vl_NbMach != 0)
			vl_NbMach_Expected = vl_NbMach;
		else
			vl_NbMach_Expected = 9999;

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		sprintf(vl_Str_NbMach, "%d", vl_NbMach);
		sprintf(vl_Str_NbMach_Ex, "%d", vl_NbMach_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, vl_Str_NbMach,
				vl_Str_NbMach_Ex, NULL  );



}	/* fin test_xzsa18 */


void test_xzsa19(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomSite 		vl_NomSite;
	int			vl_NbSite		= -1;
	int			vl_NbSite_Expected	= 9999;
	char 			vl_Str_NbSite[10];
	char 			vl_Str_NbSite_Ex[10];



	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_19.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSC_04NomSite(vl_NomSite);
		vl_ValRet = XZSA_19ListeMachineSite(
			vl_NomSite, NULL, &vl_NbSite,NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_19.. 
		*		avec XZSAV_InitOk = XDC_VRAI
		*  avec un Nom de Site connu
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSC_04NomSite(vl_NomSite);
		vl_ValRet = XZSA_19ListeMachineSite(
			vl_NomSite, NULL, &vl_NbSite,NULL);
		if (vl_NbSite != 9999)
			vl_NbSite_Expected = vl_NbSite;
		else
			vl_NbSite_Expected = 9999;

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		sprintf(vl_Str_NbSite, "%d", vl_NbSite);
		sprintf(vl_Str_NbSite_Ex, "%d", vl_NbSite_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, vl_Str_NbSite,
				vl_Str_NbSite_Ex, NULL  );


		/* -----------------------------------------*/	
		/* Appel fonction XZSA_19.. 
		*		avec XZSAV_InitOk = XDC_VRAI
		*  avec un Nom de Site inconnu
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_19ListeMachineSite(
			NULL, NULL, &vl_NbSite,NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_NOA_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL);

}	/* fin test_xzsa19 */



void test_xzsa21(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];



		/* -----------------------------------------*/	
		/* Appel fonction XZSA_21.. 
				avec XZSAV_InitOk = XDC_VRAI
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_21LibereRessourcesAdmin();

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL);


}	/* fin test_xzsa21 */


void test_xzsa22(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomSite 		vl_NomSite;


	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_22.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSC_04NomSite(vl_NomSite);
		vl_ValRet = XZSA_22ExisterSite(vl_NomSite);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_22.. 
				avec XZSAV_InitOk = XDC_VRAI
		* et Nom Site Connu
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSC_04NomSite(vl_NomSite);
		vl_ValRet = XZSA_22ExisterSite(vl_NomSite);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL  );

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_22.. 
				avec XZSAV_InitOk = XDC_VRAI
		* et Nom Site InConnu
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_22ExisterSite(NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_NOS_INV;
		
		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL  );



}	/* fin test_xzsa22 */


void test_xzsa23(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomMachine        vl_NomMach;
	XZSCT_NomSite       	vl_NomSite;
	XZSCT_NomSite       	vl_NomSite_Ex 		= "CI";

	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_23.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_23SiteMachine(
					vl_NomMach, vl_NomSite);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_23.. 
				avec XZSAV_InitOk = XDC_VRAI
		*  et Nom machine  connue 
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_23SiteMachine(
					vl_NomMach, vl_NomSite);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, vl_NomSite,
				vl_NomSite_Ex, NULL  );

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_23.. 
				avec XZSAV_InitOk = XDC_VRAI
		*  et Nom machine inconnue 
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSC_07NomMachine(vl_NomMach);
		vl_ValRet = XZSA_23SiteMachine(
					NULL, vl_NomSite);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_NOM_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL );




}	/* fin test_xzsa23 */


void test_xzsa24(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	int			vl_NbEnt		= -1;
	int			vl_NbEnt_Expected	= 9999;
	char 			vl_Str_NbEnt[10];
	char 			vl_Str_NbEnt_Ex[10];


	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_24.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSA_24ListeEntite( NULL, &vl_NbEnt,NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_24.. 
				avec XZSAV_InitOk = XDC_VRAI
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_24ListeEntite( NULL, &vl_NbEnt,NULL);
		if (vl_NbEnt != -1)
			vl_NbEnt_Expected = vl_NbEnt;
		else
			vl_NbEnt_Expected = 9999;

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		sprintf(vl_Str_NbEnt, "%d", vl_NbEnt);
		sprintf(vl_Str_NbEnt_Ex, "%d", vl_NbEnt_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, vl_Str_NbEnt,
				vl_Str_NbEnt_Ex, NULL  );


}	/* fin test_xzsa24 */


void test_xzsa25(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomEntite		vl_NomEntite		= "laser";
	int			vl_TypeEntite		= XZSAC_TYPE_PERI;


	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_25.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSA_25ExisterEntite(vl_NomEntite, vl_TypeEntite);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_25.. 
				avec XZSAV_InitOk = XDC_VRAI
		* et Nom Site Connu
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_25ExisterEntite(vl_NomEntite, vl_TypeEntite);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL  );

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_25.. 
				avec XZSAV_InitOk = XDC_VRAI
		* et Nom Entite InConnu
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_25ExisterEntite(NULL, vl_TypeEntite);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_NOE_INV;
		
		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL  );



}	/* fin test_xzsa25 */


void test_xzsa28(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomEntite		vl_NomEntite		= "laser";
	int			vl_TypeEntite		=  XZSAC_TYPE_PERI;
	XZSCT_NomSite		vl_NomSite;
	XZSCT_NomSite		vl_NomSite_Ex		= "CI";


	if (vg_Test20 == 0)
	{
		/* -----------------------------------------*/	
		/* Appel fonction XZSA_28.. avec XZSAV_InitOk = XDC_FAUX
		*/
		vl_ValRet = XZSA_28SiteEntite(vl_NomEntite, 
			vl_TypeEntite, vl_NomSite );	

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_INI_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );
	}

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_28.. 
				avec XZSAV_InitOk = XDC_VRAI
		* et Nom Entite Connu
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_28SiteEntite(vl_NomEntite, 
			vl_TypeEntite, vl_NomSite );	

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, 
				vl_NomSite, vl_NomSite_Ex ,NULL  );

		/* -----------------------------------------*/	
		/* Appel fonction XZSA_28.. 
				avec XZSAV_InitOk = XDC_VRAI
		* et Nom Entite InConnu
		*/
		if (vg_Test20 == 0)
		{
			test_xzsa20(20, XDC_FAUX);
		}

		vl_ValRet = XZSA_28SiteEntite(NULL, 
			vl_TypeEntite, vl_NomSite );	

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSAC_NOE_INV;
		
		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
				vl_Str_ValRet_Ex, NULL  );


}	/* fin test_xzsa28 */




/******* Fonction test_xxxx  ********/
int test_xzsa(int Num_Test)
{
	
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];


        /** Suivant le test demande **/
        switch (Num_Test)
        {

        case 1:

		/* Appel fonction de test */
		test_xzsa01(Num_Test);

		break;

        case 2:

		/* Appel fonction de test */
		test_xzsa02(Num_Test);

		break;

        case 3:
		/* Appel fonction de test */
		test_xzsa03(Num_Test);

		break;

        case 4:
		/* Appel fonction de test */
		test_xzsa04(Num_Test);

		break;

        case 5:
		/* Appel fonction de test */
		test_xzsa05(Num_Test);

		break;

        case 6:
		/* Appel fonction de test */
		test_xzsa06(Num_Test);

		break;

        case 7:
		/* Appel fonction de test */
		test_xzsa07(Num_Test);

		break;

        case 8:
		/* Appel fonction de test */
		test_xzsa08(Num_Test);

		break;


        case 11:
		/* Appel fonction de test */
		test_xzsa11(Num_Test);

		break;

        case 12:
		/* Appel fonction de test */
		test_xzsa12(Num_Test);

		break;

        case 13:
		/* Appel fonction de test */
		test_xzsa13(Num_Test);

		break;

        case 17:
		/* Appel fonction de test */
		test_xzsa17(Num_Test);

		break;

        case 18:
		/* Appel fonction de test */
		test_xzsa18(Num_Test);

		break;

        case 19:
		/* Appel fonction de test */
		test_xzsa19(Num_Test);

		break;



        case 20:
		/* Appel fonction de test */
		test_xzsa20(Num_Test, XDC_VRAI);

		break;


        case 21:
		/* Appel fonction de test */
		test_xzsa21(Num_Test);

		break;

        case 22:
		/* Appel fonction de test */
		test_xzsa22(Num_Test);

		break;

        case 23:
		/* Appel fonction de test */
		test_xzsa23(Num_Test);

		break;


        case 24:
		/* Appel fonction de test */
		test_xzsa24(Num_Test);

		break;

        case 25:
		/* Appel fonction de test */
		test_xzsa25(Num_Test);

		break;

        case 28:
		/* Appel fonction de test */
		test_xzsa28(Num_Test);

		break;

        default :
                break;
        }

}       /* Fin test_xzsc **/


/********  Affichage Menu **********/
int Affiche_Menu()
{
        char *  choix;
        int     retour_flush;
        char *  retour_choix;
        int     vl_NumTest;

	fprintf(FIC_DIAL,"\n\n\n");
	fprintf(FIC_DIAL,"\tXZSA_01ListeMachine\t\t\t--> 1\n");
	fprintf(FIC_DIAL,"\tXZSA_02ListeSysteme\t\t\t--> 2\n");
	fprintf(FIC_DIAL,"\tXZSA_03ListeTacheMachine\t\t--> 3\n");
	fprintf(FIC_DIAL,"\tXZSA_04ListeTacheMachineSSyst\t\t--> 4\n");
	fprintf(FIC_DIAL,"\tXZSA_05ExisterMachine\t\t\t--> 5\n");
	fprintf(FIC_DIAL,"\tXZSA_06ExisterSSysteme\t\t\t--> 6\n");
	fprintf(FIC_DIAL,"\tXZSA_07ExisterTache\t\t\t--> 7\n");
	fprintf(FIC_DIAL,"\tXZSA_08NomTacheNomSSysteme\t\t--> 8\n");
	fprintf(FIC_DIAL,"\tXZSA_11TacheTemporaire\t\t\t--> 11\n");
	fprintf(FIC_DIAL,"\tXZSA_12ListeTacheTemporaire\t\t--> 12\n");
	fprintf(FIC_DIAL,"\tXZSA_13ListeTachePermanente\t\t--> 13\n");
	fprintf(FIC_DIAL,"\tXZSA_17TypeMachine\t\t\t--> 17\n");
	fprintf(FIC_DIAL,"\tXZSA_18ListeSite\t\t\t--> 18\n");
	fprintf(FIC_DIAL,"\tXZSA_19ListeMachineSite\t\t\t--> 19\n");
	fprintf(FIC_DIAL,"\tXZSA_20InitAdmin\t\t\t--> 20\n");
	fprintf(FIC_DIAL,"\tXZSA_21LibereRessourcesAdmin\t\t--> 21\n");
	fprintf(FIC_DIAL,"\tXZSA_22ExisterSite\t\t\t--> 22\n");
	fprintf(FIC_DIAL,"\tXZSA_23SiteMachine\t\t\t--> 23\n");
	fprintf(FIC_DIAL,"\tXZSA_24ListeEntite\t\t\t--> 24\n");
	fprintf(FIC_DIAL,"\tXZSA_25ExisterEntite\t\t\t--> 25\n");
	fprintf(FIC_DIAL,"\tXZSA_28SiteEntite\t\t\t--> 28\n");
        
	fprintf(FIC_DIAL,"\n\tSORTIE\t\t\t\t\t--> 99\n\n");

        /** Recuperation du choix ***/
        retour_choix= fgets(choix,3,stdin);
        retour_flush = fflush(stdin);

        /** Conversion Chaine en INt **/
        sscanf(choix,"%d",&vl_NumTest);
	free(choix);

        /** Traitement suivant le choix **/
        if (vl_NumTest >0 && vl_NumTest <29)
        {
                test_xzsa(vl_NumTest);
                return(XDC_OK);
        }

        return (XDC_NOK);

}       /* Fin Affiche_Menu ****/



/************ Programmme principal *****/
main (int argc, char *argv[])
{

	int			return_val;
	int			vl_ValRet = XDC_OK;
	char *  		Nom_Appli = "migrazur";
	XZSCT_NomMachine        NomMachine = "PODN1";
        XZSCT_NomTache		Nom_Tache = "tshor.x";
	int			Retour_Menu = XDC_OK;



	/*  Recuperation de l'argument qui est le nom 
	*	du terminal ou l'on execute le programme de
	*   	test. Ceci afin de rediriger la sortie standart **/
	/* Ouverture du fichier de dialogue */


	if( (FIC_DIAL = fopen(argv[argc-1],"w")) == (FILE *) NULL )
	{
		exit(0);
	}	

	/****  Connexion au RTServer ***/
	XZSC_01InitCnxSup (Nom_Appli,Nom_Tache,
                        XZSCC_INTERDIRE,
                        XZSCC_INTERDIRE,
                        XZSCC_INTERDIRE,
                        NULL,"/dev/null");

	/** Ouverture du fichier Resultat **/
	Fic_Result = open_fic_result(Nom_Fic_Result);
 
	/**  Boucle dans le Menu ***/	
	while (Retour_Menu != XDC_NOK)
	{
		/** Affichage Menu ****/
		Retour_Menu = Affiche_Menu();
	}


	/** Fermeture du Fichier Resultat **/
	close_fichier(Fic_Result);

	/* Fermeture du fichier de dialogue **/
	fclose(FIC_DIAL);

}	/* fin main */

