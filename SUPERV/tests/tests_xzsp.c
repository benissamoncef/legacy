/*********************************************************
*	Fichier permettant de tester le bon fonctionnement
*	des modules d 'interfaces XZSP 
*
***********************************************************/

#include <stdio.h>
#include <rtworks/common.h>
#include <rtworks/ipc.h>
#include <varargs.h>

#include "xdc.h"
#include "xdf.h"
#include "xzsm.h"
#include "xzst.h"
#include "xzsp.h"
#include "xzss.h"


#define REPERTOIRE      "/users/milleville/migrazur/SUPERV/tests"
#define Nom_Fic_Result  REPERTOIRE"/tests_result.xzsp"
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


/***********  Fonction Utile dans l'appel de XZSP_07 ***/
void  Affiche_Result_XZSP07(int va_NumText, XZSPT_Horodate va_Horodate, 
			int va_NumEnr, char * pa_Text)
{

	fprintf (Fic_Result,
                        "\tEnregistrement Lu : \n \
		Num Texte : %d -- Horodate  %s -- Num Enr  %d-- Texte %s \n ",
			va_NumText, va_Horodate, va_NumEnr , pa_Text);
}

/***********  Fonction Utile dans l'appel de XZSP_08 ***/
void  Affiche_Result_XZSP08(XZSPT_ComposantVL va_Enr, int va_NbElt)
{

	fprintf (Fic_Result,
                        "\tEnregistrement Lu : \n \
	\tType : %s -- Machine %s -- Fichier %s -- \n \
			Version %s --  Horodate %s \n",
			va_Enr.TypeFic, va_Enr.NomMachine, va_Enr.NomFichier,
			va_Enr.NumVersion, va_Enr.Horodate);
}


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
                        "XZSP_%02u  (Donnee Recue  %s)\
                                \n\t\t--> OK \n",va_NumTest, vl_Donnees);

        }
        else
                fprintf (Fic_Result,
                        "XZSP_%02u  (Donnee Recue  %s)\
                                \n\t\t--> NOK \n",va_NumTest, vl_Donnees);


}       /* Affiche_Result**/


/*************************************/
void test_xzsp01(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomMachine	vl_NomMachine		= "";

	/* Recuperation du Nom de la Machine **/
	XZSC_07NomMachine(vl_NomMachine);

	/* Appel de la fonction XZSP_01... avec comme
	*	indicateur M/A = MArche   	*/
	vl_ValRet = XZSP_01CmdApplication(vl_NomMachine,XZSPC_MARCHE);

	/* Affectation de la valeur attendue **/
	vl_ValRet_Expected = XDC_OK;

	/* Affichage Resultat **/
	sprintf(vl_Str_ValRet, "%d", vl_ValRet);
	sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
	Affiche_Result(Num_Test, vl_Str_ValRet, vl_Str_ValRet_Ex, NULL);

}	/* fin test_xzsp01 */



/*************************************/
void test_xzsp02(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomMachine	vl_NomMachine		= "";
	XZSCT_NomSSysteme	vl_NomSSysteme		= "SUPERV";

	/* Recuperation du Nom de la Machine **/
	XZSC_07NomMachine(vl_NomMachine);

	/* Appel de la fonction XZSP_02... avec comme
	*	Nom SSysteme = SUPERV et  indicateur M/A = MArche   	*/
	vl_ValRet = XZSP_02CmdSSyst(vl_NomMachine,
			vl_NomSSysteme, XZSPC_MARCHE);

	/* Affectation de la valeur attendue **/
	vl_ValRet_Expected = XDC_OK;

	/* Affichage Resultat **/
	sprintf(vl_Str_ValRet, "%d", vl_ValRet);
	sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
	Affiche_Result(Num_Test, vl_Str_ValRet, vl_Str_ValRet_Ex, NULL);

}	/* fin test_xzsp02 */


/*************************************/
void test_xzsp03(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomMachine	vl_NomMachine		= "";
	XZSCT_NomTache	vl_NomTache		= "";

	/* Recuperation du Nom de la Machine **/
	XZSC_07NomMachine(vl_NomMachine);

	/* Recuperation du Nom de la Tache **/
	XZSC_06NomTache(vl_NomTache);

	/* Appel de la fonction XZSP_03... avec comme
	*	  indicateur M/A = MArche   	*/
	vl_ValRet = XZSP_03CmdTache(vl_NomMachine,
			vl_NomTache, XZSPC_MARCHE);

	/* Affectation de la valeur attendue **/
	vl_ValRet_Expected = XDC_OK;

	/* Affichage Resultat **/
	sprintf(vl_Str_ValRet, "%d", vl_ValRet);
	sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
	Affiche_Result(Num_Test, vl_Str_ValRet, vl_Str_ValRet_Ex, NULL);

}	/* fin test_xzsp03 */


/*************************************/
void test_xzsp04(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSCT_NomMachine	vl_NomMachine		= "";

	/* Recuperation du Nom de la Machine **/
	XZSC_07NomMachine(vl_NomMachine);

	/* Appel de la fonction XZSP_04... avec comme
	*	  indicateur M/A = MArche   	*/
	vl_ValRet = XZSP_04ArretMachine(vl_NomMachine,XZSPC_MARCHE);

	/* Affectation de la valeur attendue **/
	vl_ValRet_Expected = XDC_OK;

	/* Affichage Resultat **/
	sprintf(vl_Str_ValRet, "%d", vl_ValRet);
	sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
	Affiche_Result(Num_Test, vl_Str_ValRet, vl_Str_ValRet_Ex, NULL);

}	/* fin test_xzsp04 */


/*************************************/
void test_xzsp05(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];

	/* Appel de la fonction XZSP_05... */
	vl_ValRet = XZSP_05MajFichierAdm();

	/* Affectation de la valeur attendue **/
	vl_ValRet_Expected = XDC_OK;

	/* Affichage Resultat **/
	sprintf(vl_Str_ValRet, "%d", vl_ValRet);
	sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
	Affiche_Result(Num_Test, vl_Str_ValRet, vl_Str_ValRet_Ex, NULL);

}	/* fin test_xzsp05 */


/*************************************/
void test_xzsp07(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	int			vl_NbElt		= 0;
	int			vl_NbElt_Expected	= 0;
	char			vl_Str_NbElt[3]	= "\0";
	char			vl_Str_NbElt_Ex[3]	= "\0";


	/* Appel de la fonction XZSP_09... */
	vl_ValRet = XZSP_07ConsultJournalAdmin(&Affiche_Result_XZSP07, 
								&vl_NbElt);

	/* Affectation de la valeur attendue **/
	vl_ValRet_Expected = XDC_OK;

	/*Affectation Valeur de Retour **/
	vl_NbElt_Expected = vl_NbElt;


	/* Affichage Resultat **/
	sprintf(vl_Str_ValRet, "%d", vl_ValRet);
	sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
	sprintf(vl_Str_NbElt, "%d", vl_NbElt);
	sprintf(vl_Str_NbElt_Ex, "%d", vl_NbElt_Expected);
	Affiche_Result(Num_Test, vl_Str_ValRet, vl_Str_ValRet_Ex, 
				vl_Str_NbElt, vl_Str_NbElt_Ex, NULL);

}	/* fin test_xzsp07 */


/*************************************/
void test_xzsp08(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	int			vl_NbElt		= 0;
	int			vl_NbElt_Expected	= 0;
	char			vl_Str_NbElt[3]	= "\0";
	char			vl_Str_NbElt_Ex[3]	= "\0";

        fprintf (Fic_Result, "**********   XZSP_%02u ************* \n",Num_Test);

	/* Appel de la fonction XZSP_08... avec Ind Histo = XZSPC_HISTO */
	vl_ValRet = XZSP_08ListeVersionLog(&Affiche_Result_XZSP08, 
						XZSPC_HISTO, &vl_NbElt);

	/* Affectation de la valeur attendue **/
	vl_ValRet_Expected = XDC_OK;

	/*Affectation Valeur de Retour **/
	vl_NbElt_Expected = vl_NbElt;


	/* Affichage Resultat **/
	sprintf(vl_Str_ValRet, "%d", vl_ValRet);
	sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
	sprintf(vl_Str_NbElt, "%d", vl_NbElt);
	sprintf(vl_Str_NbElt_Ex, "%d", vl_NbElt_Expected);
	Affiche_Result(Num_Test, vl_Str_ValRet, vl_Str_ValRet_Ex, 
				vl_Str_NbElt, vl_Str_NbElt_Ex, NULL);




	/* Appel de la fonction XZSP_08... avec Ind Histo = XZSPC_ACTUEL */
	vl_ValRet = XZSP_08ListeVersionLog(&Affiche_Result_XZSP08, 
						XZSPC_ACTUEL, &vl_NbElt);

	/* Affectation de la valeur attendue **/
	vl_ValRet_Expected = XDC_OK;

	/*Affectation Valeur de Retour **/
	vl_NbElt_Expected = vl_NbElt;


	/* Affichage Resultat **/
	sprintf(vl_Str_ValRet, "%d", vl_ValRet);
	sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
	sprintf(vl_Str_NbElt, "%d", vl_NbElt);
	sprintf(vl_Str_NbElt_Ex, "%d", vl_NbElt_Expected);
	Affiche_Result(Num_Test, vl_Str_ValRet, vl_Str_ValRet_Ex, 
				vl_Str_NbElt, vl_Str_NbElt_Ex, NULL);

}	/* fin test_xzsp08 */


/*************************************/
void test_xzsp09(int Num_Test)
{
	int 			vl_ValRet 		= XDC_OK;
	int 			vl_ValRet_Expected 	= XDC_OK;
	char 			vl_Str_ValRet[2];
	char 			vl_Str_ValRet_Ex[2];
	XZSPT_ComposantVL	vl_Enr;
	double                  vl_Instant ;
	int                     vl_jourSemaine ;
	#define 		C_NomMachine_1	"PODN4"
	#define 		C_NomMachine_2	"PODM1"
	#define 		C_NomFichier_1 	"tests_VerLog_1.xzsp"
	#define 		C_NomFichier_2 	"tests_VerLog_2.xzsp"
	#define 		C_NumVersion_1	"1.15"
	#define 		C_NumVersion_2	"1.16"
	#define 		C_NumVersion_3	"1.17"
	#define 		C_NumVersion_4	"1.18"
	#define 		C_TypeFichier	"EXEC"
	char			vl_Cmd[200] = "";


	/* Affectation des valeurs de l'enregistrement **/
	strcpy(vl_Enr.TypeFic, C_TypeFichier);
	XZSC_07NomMachine(vl_Enr.NomMachine);
	strcpy(vl_Enr.NomFichier,C_NomFichier_1);
		/* Lecture Horodate */
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_jourSemaine,&vl_Instant);
	XZSM_11ConversionHorodate(vl_Instant, &vl_Enr.Horodate);

	/* Enregistrer 4 versions differentes pour la meme machine   ***/
	strcpy(vl_Enr.NumVersion,C_NumVersion_1);
	vl_ValRet = XZSP_09EcritVersionLog(vl_Enr);
	strcpy(vl_Enr.NumVersion,C_NumVersion_2);
	vl_ValRet = XZSP_09EcritVersionLog(vl_Enr);
	strcpy(vl_Enr.NumVersion,C_NumVersion_3);
	vl_ValRet = XZSP_09EcritVersionLog(vl_Enr);


	/* On change le Nom du Fichier et on enregistre 2 structures **/
	strcpy(vl_Enr.NomFichier,C_NomFichier_2);
	strcpy(vl_Enr.NumVersion,C_NumVersion_1);
	vl_ValRet = XZSP_09EcritVersionLog(vl_Enr);

	/* On rechange de Nom de fichier **/
	strcpy(vl_Enr.NomFichier,C_NomFichier_1);
	strcpy(vl_Enr.NumVersion,C_NumVersion_4);
	vl_ValRet = XZSP_09EcritVersionLog(vl_Enr);

	/* On change le Nom de la Machine on enregistre 1 structures **/
	strcpy(vl_Enr.NomMachine,C_NomMachine_2);
	strcpy(vl_Enr.NomFichier,C_NomFichier_1);
	strcpy(vl_Enr.NumVersion,C_NumVersion_1);
	vl_ValRet = XZSP_09EcritVersionLog(vl_Enr);


	/* On change le Nom du Fichier et on enregistre 1 structures **/
	strcpy(vl_Enr.NomMachine,C_NomMachine_1);
	strcpy(vl_Enr.NomFichier,C_NomFichier_2);
	strcpy(vl_Enr.NumVersion,C_NumVersion_2);
	vl_ValRet = XZSP_09EcritVersionLog(vl_Enr);

	/* Affectation de la valeur attendue **/
	vl_ValRet_Expected = XDC_OK;

	/* Affichage Resultat **/
	sprintf(vl_Str_ValRet, "%d", vl_ValRet);
	sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
	Affiche_Result(Num_Test, vl_Str_ValRet, vl_Str_ValRet_Ex, NULL);

	/* Copie du Fichier cree dans le repertoire /tmp 
	* 	ceci afin de tester la fonctin XZSP_08...   */
	sprintf(vl_Cmd,"cp %s/%s %s/%s",XDC_NSUP_PATHOPER,XDF_NSUP_VERSLOG,
				XDC_REP_TMP,XDF_NSUP_VERSLOG);
	system(vl_Cmd);


}	/* fin test_xzsp09 */


/******* Fonction test_xxxx  ********/
int test_xzsp(int Num_Test)
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
		test_xzsp01(Num_Test);

		break;

        case 2:

		/* Appel fonction de test */
		test_xzsp02(Num_Test);

		break;

        case 3:
		/* Appel fonction de test */
		test_xzsp03(Num_Test);

		break;

        case 4:
		/* Appel fonction de test */
		test_xzsp04(Num_Test);

		break;

        case 5:
		/* Appel fonction de test */
		test_xzsp05(Num_Test);

		break;

        case 7:
		/* Appel fonction de test */
		test_xzsp07(Num_Test);

		break;

        case 8:
		/* Appel fonction de test */
		test_xzsp08(Num_Test);

		break;


        case 9:
		/* Appel fonction de test */
		test_xzsp09(Num_Test);

		break;


        default :
                break;
        }

}       /* Fin test_xzsp **/


/********  Affichage Menu **********/
int Affiche_Menu()
{
        char *  choix;
        int     retour_flush;
        char *  retour_choix;
        int     vl_NumTest;

	fprintf(FIC_DIAL,"\n\n\n");
	fprintf(FIC_DIAL,"\tXZSP_01CmdApplication\t\t--> 1\n");
	fprintf(FIC_DIAL,"\tXZSP_02CmdSSyst\t\t\t--> 2\n");
	fprintf(FIC_DIAL,"\tXZSP_03CmdTache\t\t\t--> 3\n");
	fprintf(FIC_DIAL,"\tXZSP_04ArretMachine\t\t--> 4\n");
	fprintf(FIC_DIAL,"\tXZSP_05MajFichierAdm\t\t--> 5\n");
	fprintf(FIC_DIAL,"\tXZSP_06VersionLogicielle\t--> 6\n");
	fprintf(FIC_DIAL,"\tXZSP_07ConsulJournalAdmin\t--> 7\n");
	fprintf(FIC_DIAL,"\tXZSP_08ListeVersionLog\t\t--> 8\n");
	fprintf(FIC_DIAL,"\tXZSP_09EcritVersionLog\t\t--> 9\n");
        
	fprintf(FIC_DIAL,"\n\tSORTIE\t\t\t\t--> 99\n\n");

        /** Recuperation du choix ***/
        retour_choix= fgets(choix,3,stdin);
        retour_flush = fflush(stdin);

        /** Conversion Chaine en INt **/
        sscanf(choix,"%d",&vl_NumTest);
	free(choix);

        /** Traitement suivant le choix **/
        if (vl_NumTest >0 && vl_NumTest <10)
        {
                test_xzsp(vl_NumTest);
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

	/* Initialisation Administration */
	XZSA_20InitAdmin();


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

