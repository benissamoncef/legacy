/*********************************************************
*	Fichier permettant de tester le bon fonctionnement
*	des modulesd 'interfaces XZSS 
*
***********************************************************/

#include <stdio.h>
#include <rtworks/common.h>
#include <rtworks/ipc.h>
#include <varargs.h>

#include "xdc.h"
#include "xzsm.h"
#include "xzst.h"
#include "xzss.h"


#define REPERTOIRE      "/users/milleville/migrazur/SUPERV/tests"
#define Nom_Fic_Result  REPERTOIRE"/tests_result.xzss"
#define Mode_Write      "w"

FILE * 	FIC_DIAL;	/* Fichier de dialogue */
FILE *  Fic_Result;	/* Fichier de resultat */
FILE * 	FIC_SAUVE;	/* Fichier de savegarde  */

/* declaration de fonctions internes */
typedef char *myVaListe;

#define myVaStart(liste, paramN) va_start(liste, paramN)
#define myVaArg(liste, mode)     va_arg(liste, mode)
#define myVaEnd(liste) liste=NULL

#define	Nom_FicSauve  "tests_xzss.c"
#define	Nom_FicInexistant  "tests_inexistant.xzss"
	
int 			vg_Fichier_RS232;
char 			vg_Device[20]; 		
int			vg_Fichier_O;






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
/* int va_ValRet,
                        int va_ValRet_Expected,
                        int va_Int_Recu,
                        char * va_Char_Recu)*/
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
                        "XZSS_%02u  (Donnee Recue  %s)\
                                \n\t\t--> OK \n",va_NumTest, vl_Donnees);

        }
        else
                fprintf (Fic_Result,
                        "XZSS_%02u  (Donnee Recue  %s)\
                                \n\t\t--> NOK \n",va_NumTest, vl_Donnees);


}       /* Affiche_Result**/

void test_xzss01(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];

		/* -----------------------------------------*/	
		/* Appel fonction XZSS_01.. avec PATH=NULL*/
		vl_ValRet = XZSS_01SauveFichierSD(Nom_FicSauve,NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSSC_ARG_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );

		/* -----------------------------------------*/	
		/* Appel fonction XZSS_01.. avec PATH=NON NULL 
		*  mais fichier inexistant */
		vl_ValRet = XZSS_01SauveFichierSD(Nom_FicInexistant,REPERTOIRE);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSSC_FIC_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet,vl_Str_ValRet_Ex, NULL  );

		/* -----------------------------------------*/
		/* Appel fonction XZSS_01.. avec PATH=NON NULL
		*  et  fichier existant */

		vl_ValRet = XZSS_01SauveFichierSD(Nom_FicSauve,REPERTOIRE);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet,vl_Str_ValRet_Ex, NULL  );

}	/* fin test_xzss01 */


void test_xzss02(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];


		/* Appel fonction XZSS_02.. avec PATH=NULL*/
		vl_ValRet = XZSS_02RestaureFichierSD(Nom_FicSauve,NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSSC_ARG_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );


		/* -----------------------------------------*/	
		/* Appel fonction XZSS_02.. avec PATH=NON NULL 
		*  mais fichier inexistant */
		vl_ValRet = XZSS_02RestaureFichierSD(Nom_FicInexistant,REPERTOIRE);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSSC_FIC_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet,vl_Str_ValRet_Ex, NULL  );

		/* -----------------------------------------*/
		/* Appel fonction XZSS_02.. avec PATH=NON NULL
		*  et  fichier existant */

		vl_ValRet = XZSS_02RestaureFichierSD(Nom_FicSauve,REPERTOIRE);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet,vl_Str_ValRet_Ex, NULL  );

}	/* fin test_xzss02 */


void test_xzss03(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];

		/* Appel fonction XZSS_03.. avec PATH=NULL*/
		vl_ValRet = XZSS_03ImprimerFichierSec(
				XZSSC_TYPE_IMPRIMA,Nom_FicSauve,NULL,XDC_IMP_ASCII);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSSC_ARG_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );

		/* -----------------------------------------*/	
		/* Appel fonction XZSS_03.. avec PATH=NON NULL 
		*  mais fichier inexistant */
		vl_ValRet = XZSS_03ImprimerFichierSec(
				XZSSC_TYPE_IMPRIMA,Nom_FicInexistant,REPERTOIRE,XDC_IMP_ASCII);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSSC_FIC_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet,vl_Str_ValRet_Ex, NULL  );

		/* -----------------------------------------*/
		/* Appel fonction XZSS_03.. avec PATH=NON NULL
		*  et  fichier existant */

		vl_ValRet = XZSS_03ImprimerFichierSec(
				XZSSC_TYPE_IMPRIMA,Nom_FicSauve,REPERTOIRE,XDC_IMP_ASCII);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet,vl_Str_ValRet_Ex, NULL  );

}	/* fin test_xzss03 */

void test_xzss04(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];

		/* Appel fonction XZSS_04.. avec PATH=NULL*/
		vl_ValRet = XZSS_04ImprimerFichier(
				XZSSC_TYPE_IMPRIMA,Nom_FicSauve,NULL,XDC_IMP_ASCII);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSSC_ARG_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );

		/* -----------------------------------------*/	
		/* Appel fonction XZSS_04.. avec PATH=NON NULL 
		*  mais fichier inexistant */
		vl_ValRet = XZSS_04ImprimerFichier(
				XZSSC_TYPE_IMPRIMA,Nom_FicInexistant,REPERTOIRE,XDC_IMP_ASCII);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSSC_FIC_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet,vl_Str_ValRet_Ex, NULL  );

		/* -----------------------------------------*/
		/* Appel fonction XZSS_04.. avec PATH=NON NULL
		*  et  fichier existant */

		vl_ValRet = XZSS_04ImprimerFichier(
				XZSSC_TYPE_IMPRIMA,Nom_FicSauve,REPERTOIRE,XDC_IMP_ASCII);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet,vl_Str_ValRet_Ex, NULL  );

}	/* fin test_xzss04 */


void test_xzss05(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];

		/* Appel fonction XZSS_05   */
		vl_ValRet = XZSS_05TraitSignaux();

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet,vl_Str_ValRet_Ex, NULL  );

}	/* fin test_xzss05 */

void test_xzss06(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	FILE * 			vl_FicTmp;


		/* Appel fonction XZSS_06.. avec NomFichier=NULL*/
		vl_ValRet = XZSS_06DetruireFichier(NULL);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSSC_ARG_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );

	/* --------------------------------------------*/
		/* Appel fonction XZSS_06.. avec NomFichier=NON NULL
		*  mais fichier inexistant */
		vl_ValRet = XZSS_06DetruireFichier(Nom_FicInexistant);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSSC_FIC_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );

	/* --------------------------------------------*/
		/* Appel fonction XZSS_06.. avec NomFichier=NON NULL
		*  mais fichier existant */

		/* Creation du fichier */
		vl_FicTmp = fopen(Nom_FicInexistant,Mode_Write);
		fclose(vl_FicTmp);

		vl_ValRet = XZSS_06DetruireFichier(Nom_FicInexistant);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );

}	/* fin test_xzss06 */

void test_xzss07(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomMachine	vl_NomMac_Orig		= "PODN4";
	XZSCT_NomMachine	vl_NomMac_Dest		= "PODN4";
	char *			vl_Fichier_Origin 	
					="tests_xzss" ;
	char *			vl_Fichier_Dest		
					="tests_transfert.xzss";
	char *			vl_Path_Orig		=REPERTOIRE;
	char *			vl_Path_Dest		=REPERTOIRE;

	/* ------------------------------------*/
		/* Appel fonction XZSS_07.. avec PATH=NULL*/
		vl_ValRet = XZSS_07TransfertFichier(
			vl_NomMac_Orig,NULL,vl_Path_Orig,
			vl_NomMac_Dest,vl_Fichier_Dest,vl_Path_Dest);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSSC_ARG_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );

	/* ------------------------------------*/
		/* Appel fonction XZSS_07.. avec PATH= NON NULL*/
		vl_ValRet = XZSS_07TransfertFichier(
			vl_NomMac_Orig,vl_Fichier_Origin,vl_Path_Orig,
			vl_NomMac_Dest,vl_Fichier_Dest,vl_Path_Dest);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );



}	/* fin test_xzss07 */



void test_xzss08(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	int			vl_Vitesse		= XZSSC_RS_4800;
	int			vl_FlagParam		
					= XZSSC_8BITS_PNON_1SB;


	/* -------------------------------------*/
		/* Appel fonction XZSS_08. avec DEVICE= NULL*/
		vl_ValRet = XZSS_08OuvertureRS232(
				NULL,vl_Vitesse,
				vl_FlagParam,&vg_Fichier_RS232);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSSC_DEV_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );

	/* -------------------------------------*/
		/* Appel fonction XZSS_08. avec DEVICE= NON NULL*/
		vl_ValRet = XZSS_08OuvertureRS232(
				vg_Device,vl_Vitesse,
				vl_FlagParam,&vg_Fichier_RS232);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );


}	/* fin test_xzss08 */



void test_xzss09(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	int			vl_Vitesse		= XZSSC_RS_4800;
	int			vl_FlagParam		
					= XZSSC_8BITS_PNON_1SB;


	/* ---------------------------------------*/
		/* Appel fonction XZSS_08. avec DEVICE= NON NULL*/
		vl_ValRet = XZSS_08OuvertureRS232(
				vg_Device,vl_Vitesse,
				vl_FlagParam,&vg_Fichier_RS232);


		/* Appel fonction XZSS_09.. avec Device = NON NULL*/
		vl_ValRet = XZSS_09FermetureRS232(vg_Fichier_RS232);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
						vl_Str_ValRet_Ex, NULL  );

}	/* fin test_xzss09 */


void test_xzss10(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	char *                  vl_Buffer_RS232_Aide 
					= "Tapez coucou  S.V.P\n";
	char                   vl_Buffer_RS232_Expected[80];
	char 			vl_Buffer_RS232[80];
	int 			vl_Nb_Car		= 80;
	int			vl_Vitesse		
						= XZSSC_RS_9600;
	int			vl_FlagParam		
					= XZSSC_8BITS_PNON_1SB;


		/* Appel fonction XZSS_08. avec DEVICE= NON NULL*/
		vl_ValRet = XZSS_08OuvertureRS232(
				vg_Device,vl_Vitesse,
				vl_FlagParam,&vg_Fichier_RS232);

		/* Appel fonction XZSS_11..*/
		vl_ValRet = XZSS_11EcritureRS232(
			vg_Fichier_RS232,vl_Buffer_RS232_Aide,
				strlen(vl_Buffer_RS232_Aide));

		/* Appel fonction XZSS_10..*/
		vl_ValRet = XZSS_10LectureRS232(vg_Fichier_RS232,
				vl_Buffer_RS232, vl_Nb_Car);

		strcpy(vl_Buffer_RS232_Expected,
				vl_Buffer_RS232);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
			vl_Str_ValRet_Ex, vl_Buffer_RS232,
				vl_Buffer_RS232_Expected, NULL  );

}	/* fin test_xzss10 */


void test_xzss11(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	char *			vl_Buffer_RS232 	= "coucou";
	int			vl_Vitesse		
					= XZSSC_RS_9600;
	int			vl_FlagParam		
					= XZSSC_8BITS_PNON_1SB;



		/* Appel fonction XZSS_08. avec DEVICE= NON NULL*/
		vl_ValRet = XZSS_08OuvertureRS232(
				vg_Device,vl_Vitesse,
				vl_FlagParam,&vg_Fichier_RS232);

		/* Appel fonction XZSS_11..*/
		vl_ValRet = XZSS_11EcritureRS232(
			vg_Fichier_RS232,vl_Buffer_RS232,
				strlen(vl_Buffer_RS232));

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );

}	/* fin test_xzss11 */


void test_xzss12(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	int			vl_Flag_O		= XZSSC_FIC_WR;
	char *			vl_NomFic_O		
					= REPERTOIRE"/tests_xzss.c";

	/* -----------------------------------------*/
		/* Appel fonction XZSS_12..avec NomFichier = NULL */
		vl_ValRet = XZSS_12OuvrirFichier(
				vl_Flag_O,NULL,&vg_Fichier_O);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSSC_FIC_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );

	/* -----------------------------------------*/
		/* Appel fonction XZSS_12..avec NomFichier NON NULL */
		vl_ValRet = XZSS_12OuvrirFichier(
				vl_Flag_O,vl_NomFic_O,&vg_Fichier_O);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );


}	/* fin test_xzsss12 */


void test_xzss13(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	int			vl_Fichier_O;
	int			vl_Flag_O		= XZSSC_FIC_WR;
	char *			vl_NomFic_O		
					= REPERTOIRE"/tests_xzss.c";
	char *			vl_NomFic_O_Inex
					= REPERTOIRE"/toto.c";


	/* -----------------------------------------*/
		/* Appel fonction XZSS_12..avec NomFichier NON NULL */
		vl_ValRet = XZSS_12OuvrirFichier(
				vl_Flag_O,vl_NomFic_O_Inex,&vl_Fichier_O);
		/* Appel fonction XZSS_13..avec NomFichier = NULL */
		 vl_ValRet = XZSS_13FermerFichier(
				vl_Fichier_O);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XZSSC_FIC_INV;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );

	/* -----------------------------------------*/
		/* Appel fonction XZSS_12..avec NomFichier NON NULL */
		vl_ValRet = XZSS_12OuvrirFichier(
				vl_Flag_O,vl_NomFic_O,&vl_Fichier_O);

		/* Appel fonction XZSS_13..avec NomFichier NON NULL */
		vl_ValRet = XZSS_13FermerFichier(
				vl_Fichier_O);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );

}       /* fin test_xzsss13 */


void test_xzss14(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	int			vl_Timeout		= 5;

	/* -----------------------------------------*/
		/* Appel fonction XZSS_14..*/
		vl_ValRet = XZSS_14ArmerTimeout(
				vl_Timeout);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );
}       /* fin test_xzsss14 */


void test_xzss15(int Num_Test)
{
	int 			vl_ValRet_Timeout 		= XDC_VRAI;
	int 			vl_ValRet_Timeout_Expected 	= XDC_FAUX;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];

		/* Appel fonction XZSS_15..*/
		vl_ValRet_Timeout = XZSS_15ConsulterTimeout();

		/* Affectation de la valeur attendue **/
		vl_ValRet_Timeout_Expected = XDC_FAUX;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet_Timeout);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Timeout_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );

}       /* fin test_xzsss15 */


void test_xzss20(int Num_Test)
{
	int 			vl_ValRet_Pid 		= XDC_OK;
	int 			vl_ValRet_Pid_Expected 	= 9999;
	char 			vl_Str_ValRet[10] = "\0";
	char 			vl_Str_ValRet_Ex[10];
	XZSCT_NomTache		vl_NomTache		
				= "rlogin";
	XZSCT_NomTache		vl_NomTache_Inex	= "toto.x";

	/* ------------------------------------*/
		/* Appel fonction XZSS_20..avec Tache = TOTO */
 		vl_ValRet_Pid = XZSS_20PidTache(vl_NomTache_Inex);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Pid_Expected = 0;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet_Pid);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Pid_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );

	/* ------------------------------------*/
		/* Appel fonction XZSS_20..avec Tache NON NULL */
		vl_ValRet_Pid = XZSS_20PidTache(vl_NomTache);

		/* Affectation de la valeur attendue **/
		if (vl_ValRet_Pid != 0)
			vl_ValRet_Pid_Expected = vl_ValRet_Pid;
		else
			vl_ValRet_Pid_Expected  = 9999;


		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet_Pid);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Pid_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );

}       /* fin test_xzsss20 */


void test_xzss21(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	char * 			vl_NomRep		= REPERTOIRE;
	char **			vl_ListeFic;
	int 			i;



	/* --------------------------------------*/
		/* Appel fonction XZSS_21..avec REpertoire = NULL */
		vl_ValRet = XZSS_21LectureRepertoire(
				NULL,&vl_ListeFic);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_NOK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );

	/* --------------------------------------*/
		/* Appel fonction XZSS_21..avec REpertoire = NULL */
		vl_ValRet = XZSS_21LectureRepertoire(
				vl_NomRep,&vl_ListeFic);

		/* Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet, 
					vl_Str_ValRet_Ex, NULL  );


	/* Affichage du resultat **/
	fprintf(Fic_Result, "\t Affichage du Repertoire \n");
	while ( (strcmp(vl_ListeFic[i],NULL)))
	{
		fprintf(Fic_Result, "%s\n", vl_ListeFic[i]);
		i = i+1;			
	}

}       /* fin test_xzsss21 */






/******* Fonction test_xxxx  ********/
int test_xzss(int Num_Test)
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
		test_xzss01(Num_Test);

		break;

        case 2:

		/* Appel fonction de test */
		test_xzss02(Num_Test);

		break;

        case 3:
		/* Appel fonction de test */
		test_xzss03(Num_Test);

		break;

        case 4:
		/* Appel fonction de test */
		test_xzss04(Num_Test);

		break;

        case 5:
		/* Appel fonction de test */
		test_xzss05(Num_Test);

		break;

        case 6:
		/* Appel fonction de test */
		test_xzss06(Num_Test);

		break;

        case 7:
		/* Appel fonction de test */
		test_xzss07(Num_Test);

		break;

        case 8:
		/* Appel fonction de test */
		test_xzss08(Num_Test);

		break;

        case 9:
		/* Appel fonction de test */
		test_xzss09(Num_Test);

		break;

        case 10:
		/* Appel fonction de test */
		test_xzss10(Num_Test);

		break;

        case 11:
		/* Appel fonction de test */
		test_xzss11(Num_Test);

		break;

        case 12:
		/* Appel fonction de test */
		test_xzss12(Num_Test);

		break;

        case 13:
		/* Appel fonction de test */
		test_xzss13(Num_Test);

		break;

        case 14:
		/* Appel fonction de test */
		test_xzss14(Num_Test);

		break;

        case 15:
		/* Appel fonction de test */
		test_xzss15(Num_Test);

		break;


        case 20:
		/* Appel fonction de test */
		test_xzss20(Num_Test);

		break;


        case 21:
		/* Appel fonction de test */
		test_xzss21(Num_Test);

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
	fprintf(FIC_DIAL,"\tXZSS_01SauveFichierSD\t\t--> 1\n");
	fprintf(FIC_DIAL,"\tXZSS_02RestaureFichierSD\t--> 2\n");
	fprintf(FIC_DIAL,"\tXZSS_03ImprimerFichierSec\t--> 3\n");
	fprintf(FIC_DIAL,"\tXZSS_04ImprimerFichier\t\t--> 4\n");
	fprintf(FIC_DIAL,"\tXZSS_05TraitSignaux\t\t--> 5\n");
	fprintf(FIC_DIAL,"\tXZSS_06DetruireFichier\t\t--> 6\n");
	fprintf(FIC_DIAL,"\tXZSS_07TransfertFichier\t\t--> 7\n");
	fprintf(FIC_DIAL,"\tXZSS_08OuvertureRS232\t\t--> 8\n");
	fprintf(FIC_DIAL,"\tXZSS_09FermetureRS232\t\t--> 9\n");
	fprintf(FIC_DIAL,"\tXZSS_10LectureRS232\t\t--> 10\n");
	fprintf(FIC_DIAL,"\tXZSS_11EcritureRS232\t\t--> 11\n");
	fprintf(FIC_DIAL,"\tXZSS_12OuvrirFichier\t\t--> 12\n");
	fprintf(FIC_DIAL,"\tXZSS_13FermerFichier\t\t--> 13\n");
	fprintf(FIC_DIAL,"\tXZSS_14ArmerTimeout\t\t--> 14\n");
	fprintf(FIC_DIAL,"\tXZSS_15ConsulterTimeout\t\t--> 15\n");
	fprintf(FIC_DIAL,"\tXZSS_20PidTache\t\t\t--> 20\n");
	fprintf(FIC_DIAL,"\tXZSS_21LectureRepertoire\t--> 21\n");
        
	fprintf(FIC_DIAL,"\n\tSORTIE\t\t\t\t--> 99\n\n");

        /** Recuperation du choix ***/
        retour_choix= fgets(choix,3,stdin);
        retour_flush = fflush(stdin);

        /** Conversion Chaine en INt **/
        sscanf(choix,"%d",&vl_NumTest);
	free(choix);

        /** Traitement suivant le choix **/
        if (vl_NumTest >0 && vl_NumTest <22)
        {
                test_xzss(vl_NumTest);
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
	XZSCT_NomMachine        NomMachine = "PODN4";
        XZSCT_NomTache		Nom_Tache = "tshor.x";
	int			Retour_Menu = XDC_OK;



	/*  Recuperation de l'argument qui est le nom 
	*	du terminal ou l'on execute le programme de
	*   	test. Ceci afin de rediriger la sortie standart **/
	/* Ouverture du fichier de dialogue */

	/** Memorisation du Nom du Terminal **/
	strcpy(vg_Device,argv[argc-1]);

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

