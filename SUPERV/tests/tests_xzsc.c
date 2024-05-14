/*********************************************************
*	Fichier permettant de tester le bon fonctionnement
*	des modulesd 'interfaces XZSC 
*
***********************************************************/

#include <stdio.h>

#include "xdc.h"
#include "xzsm.h"
#include "xzst.h"
#include "xzsc.h"

#include <rtworks/common.h>
#include <rtworks/ipc.h>
#include <varargs.h>

#define REPERTOIRE      "/users/milleville/migrazur/SUPERV/tests/"
#define Nom_Fic_Result        REPERTOIRE"tests_result.xzsc"
#define Mode_Write      "w"

FILE * 	FIC_DIAL;	/* Fichier de dialogue */
FILE *  Fic_Result;	/* Fichier de resultat */

/* declaration de fonctions internes */
typedef char *myVaListe;

#define myVaStart(liste, paramN) va_start(liste, paramN)
#define myVaArg(liste, mode)     va_arg(liste, mode)
#define myVaEnd(liste) liste=NULL

/** Varaible Globale pour indiquer que le TEST 1
*	de CONNEXION est  OK  ***/
int	vg_INIT_OK = XDC_FAUX;







 
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


/**** Fonction Affiche Resumltat ***/
void Affiche_Result(int va_NumTest, ... )

{
	va_list         pl_listeArgument;
	char *          vl_Arg[10];
	int             vl_NbArg        = 0;
	int             vl_Resultat     = XDC_OK;
	char            vl_Donnees[80] = "\0";
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
			"XZSA_%02u  (Donnee Recue  %s) \n\t\t--> OK \n",
			va_NumTest, vl_Donnees);
	}
	else
		fprintf (Fic_Result,
			"XZSA_%02u  (Donnee Recue  %s) \n\t\t--> NOK \n",
			va_NumTest, vl_Donnees);

}	/* Affiche_Result**/




/******* Fonction test_xxxx  ********/
int test_xzsc(int Num_Test)
{
	int                     vl_ValRet               = XDC_OK;
	int                     vl_ValRet_Expected      = XDC_OK;
	char                    vl_Str_ValRet[2];
	char                    vl_Str_ValRet_Ex[2];
	char *  		Nom_Appli 	= "migrazur";
        XZSCT_NomTache  	Nom_Tache = 	"tshor.x";
        /*XZSCT_NomTache  	Nom_Tache = 	"tsaplg.x";*/
	int			vl_Priorite;
	int			vl_Priorite_Expected;
	char                    vl_Str_Prio[2];
	char                    vl_Str_Prio_Ex[2];
	int			vl_Periode;
	int			vl_Periode_Expected;
	char                    vl_Str_Periode[2];
	char                    vl_Str_Periode_Ex[2];
	XZSCT_NomSite		vl_NomSite;
	XZSCT_NomSite		vl_NomSite_Ex		= "CI";
	int			vl_TypeMachine;
	int			vl_TypeMachine_Expected;
	char                    vl_Str_TypeMach[2];
	char                    vl_Str_TypeMach_Ex[2];
	XZSCT_NomTache		vl_NomTache;
	XZSCT_NomTache		vl_NomTache_Ex		= "tshor.x";
	XZSCT_NomMachine	vl_NomMachine;
	XZSCT_NomMachine	vl_NomMachine_Ex		= "PODN1";
	XZSCT_NomPeriph		vl_NomPeriphSR;
	XZSCT_NomPeriph		vl_NomPeriphSR_Ex	= "DAT";
	XZSCT_NomMachine	vl_NomMachineImp;
	XZSCT_NomMachine	vl_NomMachineImp_Ex	="SDDN1";
	XZSCT_NomPeriph		vl_NomImprimante;
	XZSCT_NomPeriph		vl_NomImprimante_Ex	="laser";
	char 			vl_PathTrace[XDC_PATH_ABS_SIZE] = "";
	char 			vl_PathTrace_Ex[XDC_PATH_ABS_SIZE] = "";
	int 			vl_TailleFic;
	int 			vl_TailleFic_Expected 		= 100000;
	char                    vl_Str_Taille[10];
	char                    vl_Str_Taille_Ex[10];
	int 			vl_MaxTailleFic;
	int 			vl_MaxTailleFic_Expected	= 100000;
	char                    vl_Str_MaxTaille[10];
	char                    vl_Str_MaxTaille_Ex[10];
	int			vl_NbEcran;
	int			vl_NbEcran_Expected		= 2;
	char                    vl_Str_NbEcran[2];
	char                    vl_Str_NbEcran_Ex[2];
	XZSCT_NomPeriph         vl_NomImprimanteSec; 
	XZSCT_NomPeriph         vl_NomImprimanteSec_Ex	= "laser"; 
	XZSCT_TypeProfil	vl_ProfilMachine;
	XZSCT_TypeProfil	vl_ProfilMachine_Expected;
	char                    vl_Str_Profil[2];
	char                    vl_Str_Profil_Ex[2];
	XZSCT_NomMachine        vl_NomMachineRTserver;
	XZSCT_NomMachine        vl_NomMachineRTserver_Ex	= "PODN1";

	/** Suivant le test demande **/
	switch (Num_Test)
	{
	
	case 1:
		/** Appel Fonction XZSC_.. **/
		vl_ValRet = 
			XZSC_01InitCnxSup (Nom_Appli,Nom_Tache,
                        XZSCC_INTERDIRE,
                        XZSCC_INTERDIRE,
                        XZSCC_INTERDIRE,
                        NULL,"/dev/null");

		/** Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/* Affichage Resultat **/
		sprintf(vl_Str_ValRet, "%d", vl_ValRet);
		sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
		Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, NULL);

		if (vl_ValRet == XDC_OK)
			vg_INIT_OK = XDC_VRAI;

		break;

	case 2:
		/** Appel Fonction XZSC_.. **/
		vl_ValRet = 
			XZSC_02Priorite(&vl_Priorite);

		/** Affectation de la valeur attendue **/
		if (vg_INIT_OK == XDC_VRAI)
		{
			vl_ValRet_Expected = XDC_OK;
			vl_Priorite_Expected = vl_Priorite;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			sprintf(vl_Str_Prio, "%d", vl_Priorite);
			sprintf(vl_Str_Prio_Ex, "%d", vl_Priorite_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, vl_Str_Prio,
					vl_Str_Prio_Ex, NULL  );
		}
		else
		{
			vl_ValRet_Expected = XZSCC_INI_INV;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, NULL);
		}
 
		break;

	case 3:
		/** Appel Fonction XZSC_.. **/
		vl_ValRet = 
			XZSC_03PeriodeTestReseau(&vl_Periode);

		/** Affectation de la valeur attendue **/
		if (vg_INIT_OK == XDC_VRAI)
		{
			vl_ValRet_Expected = XDC_OK;
			vl_Periode_Expected = vl_Periode;
		
			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			sprintf(vl_Str_Periode, "%d", vl_Periode);
			sprintf(vl_Str_Periode_Ex, "%d", vl_Periode_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, vl_Str_Periode,
					vl_Str_Periode_Ex, NULL  );
		}
		else
		{
			vl_ValRet_Expected = XZSCC_INI_INV;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, NULL);
		}

		break;

	case 4:
		/** Appel Fonction XZSC_.. **/
		vl_ValRet = 
			XZSC_04NomSite(vl_NomSite);

		/** Affectation de la valeur attendue **/
		if (vg_INIT_OK == XDC_VRAI)
		{
			vl_ValRet_Expected = XDC_OK;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, vl_NomSite,
					vl_NomSite_Ex, NULL  );
		}
		else
		{
			vl_ValRet_Expected = XZSCC_INI_INV;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, NULL);
		}

		break;

	case 5:
		/** Appel Fonction XZSC_.. **/
		vl_ValRet = 
			XZSC_05TypeMachine(&vl_TypeMachine);

		/** Affectation de la valeur attendue **/
		if (vg_INIT_OK == XDC_VRAI)
		{
			vl_ValRet_Expected = XDC_OK;
			vl_TypeMachine_Expected = vl_TypeMachine;
		
			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			sprintf(vl_Str_TypeMach, "%d", vl_TypeMachine);
			sprintf(vl_Str_TypeMach_Ex, "%d", 
						vl_TypeMachine_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, vl_Str_TypeMach,
					vl_Str_TypeMach_Ex, NULL  );
		}
		else
		{
			vl_ValRet_Expected = XZSCC_INI_INV;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, NULL);
		}

		break;


	case 6:
		/** Appel Fonction XZSC_.. **/
		vl_ValRet = 
			XZSC_06NomTache(vl_NomTache);

		/** Affectation de la valeur attendue **/
		if (vg_INIT_OK == XDC_VRAI)
		{
			vl_ValRet_Expected = XDC_OK;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, vl_NomTache,
					vl_NomTache_Ex, NULL  );
		}
		else
		{
			vl_ValRet_Expected = XZSCC_INI_INV;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, NULL);
		}

		break;

	case 7:
		/** Appel Fonction XZSC_.. **/
		vl_ValRet = 
			XZSC_07NomMachine(vl_NomMachine);

		/** Affectation de la valeur attendue **/
		if (vg_INIT_OK == XDC_VRAI)
		{
			vl_ValRet_Expected = XDC_OK;


			
			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, vl_NomMachine,
					vl_NomMachine_Ex, NULL  );
		}
		else
		{
			vl_ValRet_Expected = XZSCC_INI_INV;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, NULL);
		}

		break;

	case 8:
		/** Appel Fonction XZSC_.. **/
		vl_ValRet = 
			XZSC_08NomPeriphSR(vl_NomPeriphSR);

		/** Affectation de la valeur attendue **/
		if (vg_INIT_OK == XDC_VRAI)
		{
			vl_ValRet_Expected = XDC_OK;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, vl_NomPeriphSR,
					vl_NomPeriphSR_Ex, NULL  );
		}
		else
		{
			vl_ValRet_Expected = XZSCC_INI_INV;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, NULL);
		}

		break;

	case 10:
		/** Appel Fonction XZSC_.. **/
		vl_ValRet = 
			XZSC_10NomMachineImp(vl_NomMachineImp);

		/** Affectation de la valeur attendue **/
		if (vg_INIT_OK == XDC_VRAI)
		{
			vl_ValRet_Expected = XDC_OK;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, vl_NomMachineImp,
					vl_NomMachineImp_Ex, NULL  );
		}
		else
		{
			vl_ValRet_Expected = XZSCC_INI_INV;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, NULL);
		}

		break;

	case 11:
		/** Appel Fonction XZSC_.. **/
		vl_ValRet = 
			XZSC_11NomImprimante(vl_NomImprimante);

		/** Affectation de la valeur attendue **/
		if (vg_INIT_OK == XDC_VRAI)
		{
			vl_ValRet_Expected = XDC_OK;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, vl_NomImprimante ,
					vl_NomImprimante_Ex, NULL  );
		}
		else
		{
			vl_ValRet_Expected = XZSCC_INI_INV;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, NULL);
		}

		break;

	case 13:
		/** Appel Fonction XZSC_.. **/
		vl_ValRet = 
			XZSC_13InfoFicTrace(
				vl_PathTrace,
				&vl_TailleFic,
				&vl_MaxTailleFic);

		/** Affectation de la valeur attendue **/
		if (vg_INIT_OK == XDC_VRAI)
		{
			vl_ValRet_Expected = XDC_OK;

			strcpy(vl_PathTrace_Ex,vl_PathTrace);

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			sprintf(vl_Str_Taille, "%d", vl_TailleFic);
			sprintf(vl_Str_Taille_Ex, "%d", vl_TailleFic_Expected);
			sprintf(vl_Str_MaxTaille, "%d", vl_MaxTailleFic);
			sprintf(vl_Str_MaxTaille_Ex, "%d",
						vl_MaxTailleFic_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, 
					vl_PathTrace,vl_PathTrace_Ex,
					vl_Str_Taille, vl_Str_Taille_Ex, 
					vl_Str_MaxTaille,
					vl_Str_MaxTaille_Ex, NULL  );
		}
		else
		{
			vl_ValRet_Expected = XZSCC_INI_INV;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, NULL);
		}

		break;

	case 15:
		/** Appel Fonction XZSC_.. **/
		vl_ValRet = 
			XZSC_15NomImprimanteSecours(
				vl_NomImprimanteSec);

		/** Affectation de la valeur attendue **/
		if (vg_INIT_OK == XDC_VRAI)
		{
			vl_ValRet_Expected = XDC_OK;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, vl_NomImprimanteSec,
					vl_NomImprimanteSec_Ex, NULL  );
		}	
		else
		{
			vl_ValRet_Expected = XZSCC_INI_INV;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, NULL);
		}

		break;

	case 16:
		/** Appel Fonction XZSC_.. **/
		vl_ValRet = 
			XZSC_16ProfilMachine(&vl_ProfilMachine);

		/** Affectation de la valeur attendue **/
		if (vg_INIT_OK == XDC_VRAI)
		{
			vl_ValRet_Expected = XDC_OK;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			sprintf(vl_Str_Profil, "%d", vl_ProfilMachine);
			sprintf(vl_Str_Profil_Ex, "%d", 
					vl_ProfilMachine_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, vl_Str_Profil,
					vl_Str_Profil_Ex, 
					 NULL  );
		}
		else
		{
			vl_ValRet_Expected = XZSCC_INI_INV;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, NULL);
		}

		break;

	case 20:
		/** Appel Fonction XZSC_.. **/
		vl_ValRet = 
			XZSC_20TestConnexion();

		/** Affectation de la valeur attendue **/
		vl_ValRet_Expected = XDC_OK;

		/** Appel affichage resultat **/
		Affiche_Result(Num_Test, vl_ValRet,
			vl_ValRet_Expected,vl_ValRet, NULL);


		break;

	case 21:
		/** Appel Fonction XZSC_.. **/
		vl_ValRet = 
			XZSC_21NomMachineRTserver(
				vl_NomMachineRTserver);

		/** Affectation de la valeur attendue **/
		if (vg_INIT_OK == XDC_VRAI)
		{
			vl_ValRet_Expected = XDC_OK;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, vl_NomMachineRTserver,
					vl_NomMachineRTserver_Ex, NULL  );
		}
		else
		{
			vl_ValRet_Expected = XZSCC_INI_INV;

			/* Affichage Resultat **/
			sprintf(vl_Str_ValRet, "%d", vl_ValRet);
			sprintf(vl_Str_ValRet_Ex, "%d", vl_ValRet_Expected);
			Affiche_Result(Num_Test, vl_Str_ValRet,
					vl_Str_ValRet_Ex, NULL);
		}

		break;


	default :
		break;
	}	

}	/* Fin test_xzsc **/


/********  Affichage Menu **********/
int Affiche_Menu()
{
	char * 	choix;
	int 	retour_flush;
	char * 	retour_choix;
	int 	vl_NumTest;

	fprintf(FIC_DIAL,"\n\n\n");
	fprintf(FIC_DIAL,"\tXZSC_01InitCnxSup\t\t--> 1\n");
	fprintf(FIC_DIAL,"\tXZSC_02Priorite\t\t\t--> 2\n");
	fprintf(FIC_DIAL,"\tXZSC_03PeriodeTestReseau\t--> 3\n");
	fprintf(FIC_DIAL,"\tXZSC_04NomSite\t\t\t--> 4\n");
	fprintf(FIC_DIAL,"\tXZSC_05TypeMachine\t\t--> 5\n");
	fprintf(FIC_DIAL,"\tXZSC_06NomTache\t\t\t--> 6\n");
	fprintf(FIC_DIAL,"\tXZSC_07NomMachine\t\t--> 7\n");
	fprintf(FIC_DIAL,"\tXZSC_08NomPeriphSR\t\t--> 8\n");
	fprintf(FIC_DIAL,"\tXZSC_10NomMachineImp\t\t--> 10\n");
	fprintf(FIC_DIAL,"\tXZSC_11NomImprimante\t\t--> 11\n");
	fprintf(FIC_DIAL,"\tXZSC_13InfoFicTrace\t\t--> 13\n");
	fprintf(FIC_DIAL,"\tXZSC_15NomImprimanteSecours\t--> 15\n");
	fprintf(FIC_DIAL,"\tXZSC_16ProfilMachine\t\t--> 16\n");
	fprintf(FIC_DIAL,"\tXZSC_20TestConnexion\t\t--> 20\n");
	fprintf(FIC_DIAL,"\tXZSC_21NomMachineRTserver\t--> 21\n");
	
	fprintf(FIC_DIAL,"\n\tSORTIE\t\t\t\t--> 99\n\n");

	/** Recuperation du choix ***/
	retour_choix= fgets(choix,3,stdin);
	retour_flush = fflush(stdin);

	/** Conversion Chaine en INt **/
	sscanf(choix,"%d",&vl_NumTest);

	/** Traitement suivant le choix **/
	if (vl_NumTest >0 && vl_NumTest <22)
	{	
		test_xzsc(vl_NumTest);
		return(XDC_OK);
	}	

	return (XDC_NOK);

}	/* Fin Affiche_Menu ****/




/************ Programmme principal *****/
main (int argc, char *argv[])
{

	int			return_val;
	int			vl_ValRet = XDC_OK;
	char *  		Nom_Appli = "migrazur";
	XZSCT_NomMachine        NomMachine = "PODN4";
	int			Retour_Menu = XDC_OK;



	/*  Recuperation de l'argument qui est le nom 
	*	du terminal ou l'on execute le programme de
	*   	test. Ceci afin de rediriger la sortie standart **/
	/* Ouverture du fichier de dialogue */

	if( (FIC_DIAL = fopen(argv[argc-1],"w")) == (FILE *) NULL )
	{
		exit(0);
	}	

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

