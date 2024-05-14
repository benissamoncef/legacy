/*
*
*	Fichier permettant de lancer les differents tests
*	de la tache HORLOGE 
*	
*	Suivant l'argument en entree , on active le test
*	associe.
*	ATTENTION , cet executable est associe au fichier de 
*	commandes TEST_SHOR qui permet de declencher
*	d'autres tests concernant la tache HORLOGE, mais
*	qui ne necessitent pas d'appel de fonctions en C.
*
*	test_1 --> Test de connexion de la tache HORLOGE
*	test_2 --> Test de lecture de l'heure system
*	test_3 --> Test de la diffusion de l'horodate
*	test_4 --> Test d'une demande de synchronisation d'une tache
*	test_5 --> Test des fonctions de conversion
*			de dates .
*
************************************************************/ 

#include <stdio.h>

#include "xdc.h"
#include "xzsm.h"
#include "xzsc.h"
#include "xzst.h"

#include        <rtworks/common.h>
#include        <rtworks/ipc_proto.h>
#include        <rtworks/ipc.h>

#define REPERTOIRE 	"/users/milleville/migrazur/SUPERV/tests/"
#define Nom_Fic_Result_3 	REPERTOIRE"tests_result_3.sho"
#define Nom_Fic_Result_4 	REPERTOIRE"tests_result_4.sho"
#define Nom_Fic_Result_5 	REPERTOIRE"tests_result_5.sho"

#define Mode_Write	"w"
#define C_0_Seconde	0
#define C_x_Seconde	15555555

FILE * 	Fic_Result;




/***************
*	Fonction permettant d'ouvrir un fichier de resultat
*
**************/
FILE * open_fic_result(char * nom_fichier)
{
	FILE *f;


	f = fopen(nom_fichier,Mode_Write);
	if (f == NULL)
	{
		printf (" Probleme sur ouverture fichier resultat %s "			, nom_fichier);
	}

	return f;


}	/* fin open_fic_result */



/***************
*	Fonction permettant de fermer un fichier
*
**************/
void close_fichier(FILE * fichier)
{

	/* fermeture fichier */
	fclose(fichier);

} 	/* fin close_fichier */

/*******************
* TEST  correspondant a la connexion correcte de la tache
*   HORLOGE. C'est a dire verification de l'envoi des messages
*   de XDM_TSH_MODE et XDM_TSH_SYNC et abonnements aux divers
*   datagroups 
*
********************/
int test_1_horloge()
{

	/* Affichage trace */
        XZST_03EcritureTrace(XZSTC_INFO,
			"TEST 1 HORLOGE  -->  OK \n\n\n");

	return (XDC_OK);

}	/* fin test_1_horloge */



/*************************
*  TEST correspondant a une demande de lecture 
*  de l'heure systeme et d'une mise a jour 
*  On envoie un message XDM_TSH_SYST pour demander
*  une recuperation de l'heure systeme
*
************************/
int test_2_horloge()
{

	char * datagroup_syst = "NSUPHORL_Nice";

	/*  Attente pour permettre une synchronisation
	*    avec la tache tshor.x et la SImulation */
	sleep(5.0);

	/* Envoie du message XDM_TSH_SYST sur le 
	*  datagroup XDG_NSUPHORL_Nomsite   ****/
        if (TipcSrvMsgWrite(datagroup_syst
                       ,XDM_IdentMsg(XDM_TSH_SYST)
                        ,XDM_FLG_SRVMSGWRITE,
                        NULL) )
	{
		/* Affichage trace */
                XZST_03EcritureTrace(XZSTC_INFO,
                        "TEST 2 HORLOGE  -->  OK \n\n\n");
		return (XDC_OK);
	}


}	/* fin test_2_horloge */



/******
*   TEST correspondant a la diffusion de l'horodate 
*      C'est a dire on simule une reception du message
*	XDM_TSH_HORO sur le datagroup XDG_NSUPHORL_NomSite
*
*	Pour cela on utilise la fonction d'interface 
*	XZSM_08EmettreHorodate, en lui passant en arguments
*	un nombre de secondes.
*	
* 	On verifie en utilisant l'espion reseau que le 
*	message XDM_TSH_HORO a ete envoye avec le nombre de
*	secondes desire.
*
**************/

int test_3_horloge(double va_Nb_Secondes,int vl_ValRet_Expected)

{
	int 	vl_ValRet;

	/* Affichage trace */
        XZST_03EcritureTrace(XZSTC_INFO,
                        "TEST 3 HORLOGE  -->  OK \n\n\n");

	/* Attente pour synchronisation avec tache tshor.x  */
	sleep (5.0);

	/*  Appel de la fonction d'emission du message
	*   XDM_TSH_HORO avec un nombre de seconde non null   */
	vl_ValRet = XZSM_08EmettreHorodate(va_Nb_Secondes);

	if (vl_ValRet == vl_ValRet_Expected)
	{
		fprintf (Fic_Result, 
			"HORLOGE : Test_3 Nb Secondes = %f --> OK \n"
			,va_Nb_Secondes);

	}
	else
		fprintf (Fic_Result, 
			"HORLOGE : Test_3 Nb Secondes = %f \n\
			valeur retour : %d --> NOK \n"
			,va_Nb_Secondes, vl_ValRet);
	
	return vl_ValRet;


} /* fin test_3_horloge */  



/*******************
* TEST concernant le test de la fonction XZSM_05DemanderDiffusionMode
*   c'est a dire l'emission du message XDM_TSH_SYNC 
*
************/
int test_4_horloge(char * NomTache, char * NomMachine)
{
	int 	vl_ValRet = XDC_OK;
	char * datagroup_syst = "NSUPHORL_Nice";

	/* Affichage trace */
        XZST_03EcritureTrace(XZSTC_INFO,
                        "TEST 4 HORLOGE  -->  OK \n\n\n");

	/* Attente pour synchronisation avec tache tshor.x  */
	sleep (10.0);
	
	/* Appel de la fonction XZSM_05 */
	vl_ValRet = XZSM_05DemanderDiffusionMode(
			 NomTache,
			 NomMachine );
	
	/* Attente pour synchronisation avec tache tshor.x  */
	sleep (5.0);

	/* Envoie du message XDM_TSH_SYST sur le 
	*  datagroup XDG_NSUPHORL_Nomsite   ****/
        TipcSrvMsgWrite(datagroup_syst
                       ,XDM_IdentMsg(XDM_TSH_SYST)
                        ,XDM_FLG_SRVMSGWRITE,
                        NULL);
	
	/* Attente pour synchronisation avec tache tshor.x  */
	sleep (5.0);

	/* Appel de la fonction XZSM_05 */
	vl_ValRet = XZSM_05DemanderDiffusionMode(
			 NomTache,
			 NomMachine );

}	/* fin test_4_horloge */



/********************
* TEST   concernant les fonctions de conversion de date
*   utilisant les primitives RTWORKS
*
***********************/
int test_5_horloge()
{
	int 		vl_ValRet = XDC_OK;
		/** Nb Seconde 35jours * 24 h * 3600 S **/
	double		vl_Nb_Secondes_In = (35*24*3600);  
	XZSMT_Horodate 	vl_Horodate_Out;
	XZSMT_Horodate 	vl_Horodate_Out_Expected = "05/02/1970 00:00:00";
		/** Nb Seconde 35jours * 24 h * 3600 S **/
	double		vl_Nb_SecondesInv_In = (35*24*3600);  
	XZSMT_HorodateInv 	vl_HorodateInv_Out;
	XZSMT_HorodateInv 	vl_HorodateInv_Out_Expected =
				"19700105000000";
	double          vl_Nb_Secondes_Out;
	double          vl_Nb_Secondes_Out_Expected = 765634332.00;

	XZSMT_Horodate  vl_Horodate_In = "06/04/1994 12:12:12";
		
		/** Nb Seconde 13656 S **/
	double		vl_Nb_Secondes_DelaiIn = (13656);
	XZSMT_Delai 	vl_Delai_Out;
	XZSMT_Delai 	vl_Delai_Out_Expected = "01:03:47:36";

	XDY_Heure	pa_HeureCourante;
	int 		vl_NbSec_Out	= 0.0;
	int 		vl_NbSec_Out_Expected	= 1.0;



	/********  Test de la fonction XZSM_11  *****/
	vl_ValRet = XZSM_11ConversionHorodate(
		vl_Nb_Secondes_In,
		&vl_Horodate_Out );
	if (vl_ValRet == XDC_OK) 
	{
		/*  Test si valeur retour = valeur attendue */
		if (!strcmp(vl_Horodate_Out,vl_Horodate_Out_Expected))
		{
			/** Affichage dans fichier resultat **/
			fprintf(Fic_Result,
		"Fonction XZSM_11ConversionHorodate  -- >   OK \n\n");
		}
		else
		{
			/** Affichage dans fichier resultat **/
                     	fprintf(Fic_Result,
                     	"Fonction XZSM_11ConversionHorodate  %s  -- >   NOK \n\n",
			vl_Horodate_Out);
		}
	}

	/********** Test de la fonction XZSM_12  *********/
	vl_ValRet = XZSM_12ConversionHorodateInv(
			vl_Nb_SecondesInv_In,
			&vl_HorodateInv_Out);
	if (vl_ValRet == XDC_OK)
   	{
                    /*  Test si valeur retour = valeur attendue */
                     if (!strcmp(vl_HorodateInv_Out,vl_HorodateInv_Out_Expected))
                     {
                             /** Affichage dans fichier resultat **/
                             fprintf(Fic_Result,
                     "Fonction XZSM_12ConversionHorodateInv  -- >   OK \n\n");
                     }
                     else
                     {
                             /** Affichage dans fichier resultat **/
                             fprintf(Fic_Result,
                           "Fonction XZSM_12ConversionHorodateInv %s -- >   NOK \n\n",
				vl_HorodateInv_Out);
                     }
       	}


	/********** Test de la fonction XZSM_13  *********/
        vl_ValRet = XZSM_13ConversionHorodateSec(
                        vl_Horodate_In,
			&vl_Nb_Secondes_Out);

        if (vl_ValRet == XDC_OK)
        {
                    /*  Test si valeur retour = valeur attendue */
                     if ( vl_Nb_Secondes_Out == vl_Nb_Secondes_Out_Expected)
                     {
                             /** Affichage dans fichier resultat **/
                             fprintf(Fic_Result,
                     "Fonction XZSM_13ConversionHorodateSec % lf-- >   OK \n\n",				vl_Nb_Secondes_Out);
                     }
                     else
                     {
                             /** Affichage dans fichier resultat **/
                             fprintf(Fic_Result,
                           "Fonction XZSM_13ConversionHorodateSec %lf -- >   NOK \n\n",
                                vl_Nb_Secondes_Out);
                     }
        }


	/********  Test de la fonction XZSM_14  *****/
	vl_ValRet = XZSM_14ConversionHorodateSecDelai(
		vl_Nb_Secondes_DelaiIn,
		&vl_Delai_Out );
	if (vl_ValRet == XDC_OK) 
	{
		/*  Test si valeur retour = valeur attendue */
		if (!strcmp(vl_Delai_Out,vl_Delai_Out_Expected))
		{
			/** Affichage dans fichier resultat **/
			fprintf(Fic_Result,
		"Fonction XZSM_14ConversionHorodateiSecDelai  -- >   OK \n\n");
		}
		else
		{
			/** Affichage dans fichier resultat **/
                     	fprintf(Fic_Result,
                     	"Fonction XZSM_14ConversionHorodateSec_Delai  %s %s -- >   NOK \n\n",
			vl_Delai_Out, vl_Delai_Out_Expected);
		}
	}


	/********  Test de la fonction XZSM_16  *****/
	vl_ValRet = XZSM_16ConvHeureNbSec("","12:12:12",
						&vl_Nb_Secondes_Out);
	if (vl_ValRet == XDC_OK)
	{
		/** Affichage dans fichier resultat **/
		fprintf(Fic_Result,
	"Fonction 16XZSM_16ConvHeureNbSec  -- >   OK \n \
	(Nb Secondes %lf ) \n\n", vl_Nb_Secondes_Out);

	}
	else
	{
		/** Affichage dans fichier resultat **/
		fprintf(Fic_Result,
			"Fonction XZSM_16XZSM_16ConvHeureNbSec  -- >   NOK \n \
			(valeur retour %d) \n\n",vl_ValRet);
	}


}	/* test_5_horloge */





/**********
*
*	Programme principal pour les tests HORLOGE
*
***********/

main (int argc,char * argv[])

{

	char * 	Nom_Fic_Result;
	int 	vl_ValRet = XDC_OK;
	char *	Nom_Appli = "migrazur";
	char *	Nom_Tache = "tshor.x";
	XZSCT_NomMachine	NomMachine;
	int TailleNomMachine = sizeof(NomMachine);
	XZSCT_NomTache	NomTache = "tshor.x";

/****  Connexion au RTServer ***/
XZSC_01InitCnxSup (Nom_Appli,Nom_Tache,
			XZSCC_INTERDIRE,
			XZSCC_INTERDIRE,
			XZSCC_INTERDIRE,
			NULL,"/dev/null");

vl_ValRet = test_5_horloge();	

/** Affectation suivant le test desire */
if (!strcmp(argv[argc-1],"test_1"))
{

	/* Appel Test 1 */
 	vl_ValRet = test_1_horloge();
}
else if (!strcmp(argv[argc-1],"test_2"))
{

	/* Appel Test 2 */
 	vl_ValRet = test_2_horloge();
}
else if (!strcmp(argv[argc-1],"test_3"))
{
	Nom_Fic_Result = (char *) malloc(strlen (Nom_Fic_Result_3));
	strcpy(Nom_Fic_Result,Nom_Fic_Result_3);

	/* Ouverture fichier de resultat */
	Fic_Result = open_fic_result(Nom_Fic_Result);

	/* Emission de l'horodate avec Nombrede Secondes = 0  **/
 	vl_ValRet = test_3_horloge(C_0_Seconde,XZSMC_ARG_INV);	

	/* Emission de l'horodate avec 
	*	Nombre deSecondes /= 0 mais initialisation de la 
	*  connexion non effectuee **/
 	vl_ValRet =  test_3_horloge(C_x_Seconde,XDC_OK);


}
else if (!strcmp(argv[argc-1],"test_4"))
{
	Nom_Fic_Result = (char *) malloc(strlen (Nom_Fic_Result_4));
        strcpy(Nom_Fic_Result,Nom_Fic_Result_4);

	/* Ouverture fichier de resultat */
	Fic_Result = open_fic_result(Nom_Fic_Result);

	/* recherche du nom de la machine */
    	if(gethostname(NomMachine, TailleNomMachine) == -1) 
	{
		exit(0);
	}

	/* Emission du message XDM_TSH_SYNC */
	vl_ValRet = test_4_horloge(NomTache,NomMachine);
}
else if (!strcmp(argv[argc-1],"test_5"))
{
       Nom_Fic_Result = (char *) malloc(strlen (Nom_Fic_Result_5));
        strcpy(Nom_Fic_Result,Nom_Fic_Result_5);

	/* Ouverture fichier de resultat */
	Fic_Result = open_fic_result(Nom_Fic_Result);

	/* Emission du message XDM_TSH_SYNC */
	vl_ValRet = test_5_horloge();
}

	/* Fermeture fichier Resultat */
	close_fichier(Fic_Result);		


}	/* fin main */
