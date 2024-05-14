/*E*/
/*Fichier :  @(#)enix_don.c	1.6      Release : 1.6        Date : 09/30/13
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIX * FICHIER enix_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de gestion des donnees de la tache TENIX, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       version 1.1 17 Septembre 2007    : GTC rénovée DEM 664
* JBL		Oct 2008 : Correction reception alarme systeme
* JBL		13 Oct 2008 : Suppression code inutilise (stockage interne alarmes et alertes)
* ABE		12-11-2009 : Corrections
						 FT 1841 : Animation vue système
						 FT 3043 : vue HT non animée
						 FT 2698 : Problème de non animation des équipements du domaine autre que LT dans les vues LTN
* ADB      08-03-2010 : Animation des echangeurs
* ABE      26-03-2013 : gestion du retour à la normale avec ajout de scénarios pour le domaine exploitation
------------------------------------------------------*/

/* fichiers inclus */
 
#include "enix_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)enix_don.c	1.6 09/30/13      : enix_don" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/* AAZ Ajout GTC renovee */
EX_FONCT_INSERER   (  ENIX_DONNEES_Tube, et_ins_liste_Tube, pg_debutTube, Numero, Suivant, Suivant )
EX_FONCT_INSERER   (  ENIX_DONNEES_Echangeur, et_ins_liste_Ech, pg_debutEchangeur, NumeroDemiEch, Suivant, Suivant )
EX_FONCT_INSERER   (  ENIX_DONNEES_SIG, et_ins_liste_SIG, pg_debutSIG, NumeroAbsolu, Suivant, Suivant )
EX_FONCT_INSERER   (  ENIX_DONNEES_ECL, et_ins_liste_ECL, pg_debutECL, NumeroAbsolu, Suivant, Suivant )
EX_FONCT_INSERER   (  ENIX_DONNEES_VEN, et_ins_liste_VEN, pg_debutVEN, NumeroAbsolu, Suivant, Suivant )
EX_FONCT_INSERER   (  ENIX_DONNEES_ENR, et_ins_liste_ENR, pg_debutENR, NumeroAbsolu, Suivant, Suivant )
EX_FONCT_INSERER   (  ENIX_DONNEES_INC, et_ins_liste_INC, pg_debutINC, NumeroAbsolu, Suivant, Suivant )
EX_FONCT_INSERER   (  ENIX_DONNEES_EXP, et_ins_liste_EXP, pg_debutEXP, NumeroAbsolu, Suivant, Suivant )
EX_FONCT_INSERER   (  ENIX_DONNEES_SYS, et_ins_liste_SYS, pg_debutSYS, NumeroAbsolu, Suivant, Suivant )
EX_FONCT_INSERER   (  ENIX_DONNEES_LTN, et_ins_liste_LTN, pg_debutLTN, NumeroAbsolu, Suivant, Suivant )


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Insertion structure de donnee Sequence
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_ins_liste_Sequence ( ENIX_DONNEES_Sequence *  pa_Sequence,
                                XDY_Eqt                  va_NumOuvrage,
                                XDY_Mot                  va_TypeOuvrage,
								XDY_Mot					 va_Domaine)

/*
* ARGUMENTS EN ENTREE :
*
*  ENIX_DONNEES_Sequence *     pa_Sequence    : Structure sequence a inserer
*  XDY_Eqt                     va_NumMaitre   : Numero du tube ou echangeur auquel appartient le Sequence
*  XDY_Mot                     va_TypeMaitre  : Type echangeur ou tube
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Recherche du tube ou echangeur auquel appartient le Sequence
*   Recherche de la position d'insertion
*   Allocation memoire et insertion dans la liste
*
------------------------------------------------------*/
{
    static char *version = "@(#)enix_don.c	1.6 09/30/13      : et_ins_liste_Sequence " ;
    /* AAZ 17/09/2007 */
    ENIX_DONNEES_SIG *        pl_DonCrtSIG;
    ENIX_DONNEES_ECL *        pl_DonCrtECL;
    ENIX_DONNEES_VEN *        pl_DonCrtVEN;
    ENIX_DONNEES_ENR *        pl_DonCrtENR;
    ENIX_DONNEES_INC *        pl_DonCrtINC;
    ENIX_DONNEES_EXP *        pl_DonCrtEXP;
    ENIX_DONNEES_Sequence *   pl_ExEnr;
    ENIX_DONNEES_Sequence *   pl_Sequence;
    ENIX_DONNEES_Sequence *   pl_Enr;    
    ENIX_DONNEES_Sequence *   pl_Debut  = NULL;
    int                       vl_Trouve = XDC_NOK;    

    XZST_03EcritureTrace( XZSTC_INFO, "IN : et_ins_liste_Sequence");

    switch ( va_Domaine )
    {
	    /* 26-03-2013 : gestion du retour à la normale */
        case XDC_EQT_EXP :
            /* Recherche equipement/exploitation auquel appartient la sequence */
            for ( pl_DonCrtEXP =  pg_debutEXP;
                  pl_DonCrtEXP != NULL;
                  pl_DonCrtEXP =  pl_DonCrtEXP->Suivant)
            {
                /* Si Numero equipement/exploitation existe */
                if ( pl_DonCrtEXP->Numero == va_NumOuvrage && pl_DonCrtEXP->TypeOuvrage == va_TypeOuvrage)
                {
                    /* Mise a jour du pointeur de debut de liste sequence */
                    pl_Debut  = pl_DonCrtEXP->ListeSequence;
                    vl_Trouve = XDC_OK;
                    break;
                }
            }
            break;
	    
        case XDC_EQT_SIG :
            /* Recherche equipement/signalisation auquel appartient la sequence */
            for ( pl_DonCrtSIG =  pg_debutSIG;
                  pl_DonCrtSIG != NULL;
                  pl_DonCrtSIG =  pl_DonCrtSIG->Suivant)
            {
                /* Si Numero equipement/signalisation existe */
                if ( pl_DonCrtSIG->Numero == va_NumOuvrage && pl_DonCrtSIG->TypeOuvrage == va_TypeOuvrage)
                {
                    /* Mise a jour du pointeur de debut de liste sequence */
                    pl_Debut  = pl_DonCrtSIG->ListeSequence;
                    vl_Trouve = XDC_OK;
                    break;
                }
            }
            break;

        case XDC_EQT_ECL :
            /* Recherche equipement/eclairage auquel appartient la sequence */
            for ( pl_DonCrtECL =  pg_debutECL;
                  pl_DonCrtECL != NULL;
                  pl_DonCrtECL =  pl_DonCrtECL->Suivant)
            {
                /* Si Numero equipement/eclairage existe */
                if ( pl_DonCrtECL->Numero == va_NumOuvrage && pl_DonCrtECL->TypeOuvrage == va_TypeOuvrage)
                {
                    /* Mise a jour du pointeur de debut de liste sequence */
                    pl_Debut  = pl_DonCrtECL->ListeSequence;
                    vl_Trouve = XDC_OK;
                    break;
                }
            }
            break;

        case XDC_EQT_VEN :
            /* Recherche equipement/ventilation auquel appartient la sequence */
            for ( pl_DonCrtVEN =  pg_debutVEN;
                  pl_DonCrtVEN != NULL;
                  pl_DonCrtVEN =  pl_DonCrtVEN->Suivant)
            {
                /* Si Numero equipement/ventilation existe */
                if ( pl_DonCrtVEN->Numero == va_NumOuvrage && pl_DonCrtVEN->TypeOuvrage == va_TypeOuvrage)
                {
                    /* Mise a jour du pointeur de debut de liste sequence */
                    pl_Debut  = pl_DonCrtVEN->ListeSequence;
                    vl_Trouve = XDC_OK;
                    break;
                }
            }
            break;

        case XDC_EQT_ENR :
            /* Recherche equipement/energie auquel appartient la sequence */
            for ( pl_DonCrtENR =  pg_debutENR;
                  pl_DonCrtENR != NULL;
                  pl_DonCrtENR =  pl_DonCrtENR->Suivant)
            {
                /* Si Numero equipement/energie existe */
                if ( pl_DonCrtENR->Numero == va_NumOuvrage && pl_DonCrtENR->TypeOuvrage == va_TypeOuvrage)
                {
                    /* Mise a jour du pointeur de debut de liste sequence */
                    pl_Debut  = pl_DonCrtENR->ListeSequence;
                    vl_Trouve = XDC_OK;
                    break;
                }
            }
            break;

        case XDC_EQT_INC :
            /* Recherche equipement/protection incendie auquel appartient la sequence */
            for ( pl_DonCrtINC =  pg_debutINC;
                  pl_DonCrtINC != NULL;
                  pl_DonCrtINC =  pl_DonCrtINC->Suivant)
            {
                /* Si Numero equipement/protection incendie existe */
                if ( pl_DonCrtINC->Numero == va_NumOuvrage && pl_DonCrtINC->TypeOuvrage == va_TypeOuvrage)
                {
                    /* Mise a jour du pointeur de debut de liste sequence */
                    pl_Debut  = pl_DonCrtINC->ListeSequence;
                    vl_Trouve = XDC_OK;
                    break;
                }
            }
            break;

        default :
            break;
    }

    if ( vl_Trouve == XDC_OK )
    {
        /*A si l'allocation memoire est ok */
        if ( ( pl_Sequence = (ENIX_DONNEES_Sequence *)malloc ( sizeof(ENIX_DONNEES_Sequence) ) ) != NULL)
        {
            /*A copie enegistrement */
            memcpy ( pl_Sequence,  pa_Sequence, sizeof(*pl_Sequence) );
            
            /*A pour les enregistrements deja existant */
            for ( pl_Enr   = pl_ExEnr = pl_Debut ; 
                  pl_Enr  != NULL; 
                  pl_ExEnr = pl_Enr, pl_Enr = pl_Enr->Suivant )
            {
                /*A Si le numero du Sequence est inferieur */
                if ( pl_Sequence->NumSequence < pl_Enr->NumSequence )

                    /*A la place d'insertion est localisee : on stoppe la recherche */
                    break;
            }

            /*A si la place d'insertion est la tete de liste */
            if ( pl_Enr == pl_Debut  )
            {
                switch ( va_Domaine )
                {
	                /* 26-03-2013 : gestion du retour à la normale */
                    case XDC_EQT_EXP :
                        pl_DonCrtEXP->ListeSequence = pl_Sequence;
                        break;
                    case XDC_EQT_SIG :
                        pl_DonCrtSIG->ListeSequence = pl_Sequence;
                        break;
                    case XDC_EQT_VEN :
                        pl_DonCrtVEN->ListeSequence = pl_Sequence;
                        break;
                    case XDC_EQT_ECL :
                        pl_DonCrtECL->ListeSequence = pl_Sequence;
                        break;
                    case XDC_EQT_ENR :
                        pl_DonCrtENR->ListeSequence = pl_Sequence;
                        break;
                    case XDC_EQT_INC :
                        pl_DonCrtINC->ListeSequence = pl_Sequence;
                        break;
                }
            }
            /*A sinon insertion nominal */
            else
            {
                pl_Sequence->Suivant = pl_ExEnr->Suivant;
                pl_ExEnr->Suivant   = pl_Sequence;
            }
        }
    }

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Insertion structure de donnee Equipement
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
/* AZAZAZAZA  A voir pour parametres */
void    et_ins_liste_Equipement ( ENIX_DONNEES_Equipement *  pa_Equipement,
                                  XDY_Eqt                    va_NumMaitre,
                                  XDY_Mot                    va_TypeMaitre )

/*
* ARGUMENTS EN ENTREE :
*
*  ENIX_DONNEES_Equipement * pa_Equipement   : Structure equipement a inserer
*  XDY_Eqt                   va_NumMaitre    : Numero du tube, echangeur, ... auquel appartient l equipement
*  XDY_Mot                   va_TypeMaitre   : Type echangeur ou tube
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Recherche du tube, echangeur, domaine ... auquel appartient l'equipement
*   Recherche de la position d'insertion
*   Allocation memoire et insertion dans la liste
*
------------------------------------------------------*/
{
    static char               *version = "@(#)enix_don.c   1.3 06/18/07      : et_ins_liste_Equipement " ;
    ENIX_DONNEES_Tube *       pl_DonCrtTube;
    ENIX_DONNEES_Echangeur *  pl_DonCrtEch;
    ENIX_DONNEES_SIG *        pl_DonCrtSIG;
    ENIX_DONNEES_ECL *        pl_DonCrtECL;
    ENIX_DONNEES_VEN *        pl_DonCrtVEN;
    ENIX_DONNEES_ENR *        pl_DonCrtENR;
    ENIX_DONNEES_INC *        pl_DonCrtINC;
    ENIX_DONNEES_EXP *        pl_DonCrtEXP;
    ENIX_DONNEES_SYS *        pl_DonCrtSYS;
    ENIX_DONNEES_LTN *        pl_DonCrtLTN;
    ENIX_DONNEES_Equipement * pl_ExEnr;
    ENIX_DONNEES_Equipement * pl_Equipement;
    ENIX_DONNEES_Equipement * pl_Enr; 
    ENIX_DONNEES_Equipement * pl_Debut  = NULL;
    int                       vl_Trouve = XDC_NOK;    

    XZST_03EcritureTrace( XZSTC_INFO, "IN : et_ins_liste_Equipement");

    XZST_03EcritureTrace( XZSTC_INFO, "AZA et_ins_liste_Equipement : pa_Equipement->Domaine %d, Maitre = %d - TypeMaitre = %d", 
			pa_Equipement->Domaine, 
			va_NumMaitre,
			va_TypeMaitre);

    switch ( pa_Equipement->Domaine )
    {
        
        case XDC_EQT_TUBE :
            break;

        case XDC_EQT_ECH :
            break;

        case XDC_EQT_SIG :
            /* AAZ 20/09/2007 */
            /* Recherche du domaine auquel appartient l'equipement */
            for ( pl_DonCrtSIG = pg_debutSIG; pl_DonCrtSIG != NULL; pl_DonCrtSIG = pl_DonCrtSIG->Suivant)
            {
                /* Si Numero equipement/domaine existe */
                if ( pl_DonCrtSIG->Numero == va_NumMaitre && pl_DonCrtSIG->TypeOuvrage == va_TypeMaitre )
                {
                    /* Mise a jour du pointeur de debut de liste equipement */
                    pl_Debut  = pl_DonCrtSIG->ListeEquipement;
                    pl_DonCrtSIG->NbrEquipement++;
                    vl_Trouve = XDC_OK;
                    break;
                }
            }
            break;

        case XDC_EQT_ECL :
            /* Recherche du domaine auquel appartient l'equipement */
            for ( pl_DonCrtECL = pg_debutECL; pl_DonCrtECL != NULL; pl_DonCrtECL = pl_DonCrtECL->Suivant)
            {
                /* Si Numero equipement/domaine existe */
                if ( pl_DonCrtECL->Numero == va_NumMaitre && pl_DonCrtECL->TypeOuvrage == va_TypeMaitre )
                {
                    /* Mise a jour du pointeur de debut de liste equipement */
                    pl_Debut  = pl_DonCrtECL->ListeEquipement;
                    pl_DonCrtECL->NbrEquipement++;
                    vl_Trouve = XDC_OK;
                    break;
                }
            }
            break;

        case XDC_EQT_VEN :
            /* Recherche du domaine auquel appartient l'equipement */
            for ( pl_DonCrtVEN = pg_debutVEN; pl_DonCrtVEN != NULL; pl_DonCrtVEN = pl_DonCrtVEN->Suivant)
            {
                /* Si Numero equipement/domaine existe */
                if ( pl_DonCrtVEN->Numero == va_NumMaitre && pl_DonCrtVEN->TypeOuvrage == va_TypeMaitre )
                {
                    /* Mise a jour du pointeur de debut de liste equipement */
                    pl_Debut  = pl_DonCrtVEN->ListeEquipement;
                    pl_DonCrtVEN->NbrEquipement++;
                    vl_Trouve = XDC_OK;
                    break;
                }
            }
            break;

        case XDC_EQT_ENR :
            /* Recherche du domaine auquel appartient l'equipement */
            for ( pl_DonCrtENR = pg_debutENR; pl_DonCrtENR != NULL; pl_DonCrtENR = pl_DonCrtENR->Suivant)
            {
                /* Si Numero equipement/domaine existe */
/* modification ABE	12-11-2009 : on rajoute la condition sur le type ouvrage */
                if ( pl_DonCrtENR->Numero == va_NumMaitre && pl_DonCrtENR->TypeOuvrage == va_TypeMaitre )
                {
                    /* Mise a jour du pointeur de debut de liste equipement */
                    pl_Debut  = pl_DonCrtENR->ListeEquipement;
                    pl_DonCrtENR->NbrEquipement++;
                    vl_Trouve = XDC_OK;
                    break;
                }
            }
            break;

        case XDC_EQT_INC :
            /* Recherche du domaine auquel appartient l'equipement */
            for ( pl_DonCrtINC = pg_debutINC; pl_DonCrtINC != NULL; pl_DonCrtINC = pl_DonCrtINC->Suivant)
            {
                /* Si Numero equipement/domaine existe */
                if ( pl_DonCrtINC->Numero == va_NumMaitre && pl_DonCrtINC->TypeOuvrage == va_TypeMaitre )
                {
                    /* Mise a jour du pointeur de debut de liste equipement */
                    pl_Debut  = pl_DonCrtINC->ListeEquipement;
                    pl_DonCrtINC->NbrEquipement++;
                    vl_Trouve = XDC_OK;
                    break;
                }
            }
            break;

        case XDC_EQT_EXP :
            /* Recherche du domaine auquel appartient l'equipement */
            for ( pl_DonCrtEXP = pg_debutEXP; pl_DonCrtEXP != NULL; pl_DonCrtEXP = pl_DonCrtEXP->Suivant)
            {
                /* Si Numero equipement/domaine existe */
                if ( pl_DonCrtEXP->Numero == va_NumMaitre && pl_DonCrtEXP->TypeOuvrage == va_TypeMaitre )
                {
                    /* Mise a jour du pointeur de debut de liste equipement */
                    pl_Debut  = pl_DonCrtEXP->ListeEquipement;
                    pl_DonCrtEXP->NbrEquipement++;
                    vl_Trouve = XDC_OK;
                    break;
                }
            }
            break;

        case XDC_EQT_SYS :
            /* Recherche du domaine auquel appartient l'equipement */
            for ( pl_DonCrtSYS = pg_debutSYS; pl_DonCrtSYS != NULL; pl_DonCrtSYS = pl_DonCrtSYS->Suivant)
            {
                /* Si Numero equipement/domaine existe */
/* modification ABE	12-11-2009 : on rajoute la condition sur le type ouvrage */
                if ( pl_DonCrtSYS->Numero == va_NumMaitre && pl_DonCrtSYS->TypeOuvrage == va_TypeMaitre )
                {
                    /* Mise a jour du pointeur de debut de liste equipement */
                    pl_Debut  = pl_DonCrtSYS->ListeEquipement;
                    pl_DonCrtSYS->NbrEquipement++;
                    vl_Trouve = XDC_OK;
                    break;
                }
            }
            break;

        case XDC_EQT_LTN :
            /* Recherche du domaine auquel appartient l'equipement */
            for ( pl_DonCrtLTN = pg_debutLTN; pl_DonCrtLTN != NULL; pl_DonCrtLTN = pl_DonCrtLTN->Suivant)
            {
                /* Si Numero equipement/domaine existe */
/* modification ABE	12-11-2009 : on rajoute la condition sur le type ouvrage */
                if ( pl_DonCrtLTN->Numero == va_NumMaitre && pl_DonCrtLTN->TypeOuvrage == va_TypeMaitre )
                {
                    /* Mise a jour du pointeur de debut de liste equipement */
                    pl_Debut  = pl_DonCrtLTN->ListeEquipement;
                    pl_DonCrtLTN->NbrEquipement++;
                    vl_Trouve = XDC_OK;
                    break;
                }
            }
            break;

        default :
            break;

    }

    if ( vl_Trouve == XDC_OK )
    {
        /*A si l'allocation memoire est ok */
        if ( ( pl_Equipement = (ENIX_DONNEES_Equipement *)malloc ( sizeof(ENIX_DONNEES_Equipement) ) ) != NULL)
        {
            /*A copie enegistrement */
            memcpy ( pl_Equipement,  pa_Equipement, sizeof(*pl_Equipement) );
            /* Modifs JBL 05/11/2007 */
            pl_Equipement->TypeOuvrage = va_TypeMaitre;
			pl_Equipement->Suivant = pl_Debut;
			switch ( pa_Equipement->Domaine )
			{
				case XDC_EQT_SIG :
					pl_DonCrtSIG->ListeEquipement  = pl_Equipement;
					break;
				case XDC_EQT_ECL :
					pl_DonCrtECL->ListeEquipement  = pl_Equipement;
					break;
				case XDC_EQT_VEN :
					pl_DonCrtVEN->ListeEquipement  = pl_Equipement;
					break;
				case XDC_EQT_ENR :
					pl_DonCrtENR->ListeEquipement  = pl_Equipement;
					break;
				case XDC_EQT_INC :
					pl_DonCrtINC->ListeEquipement  = pl_Equipement;
					break;
				case XDC_EQT_EXP :
					pl_DonCrtEXP->ListeEquipement  = pl_Equipement;
					break;
				case XDC_EQT_SYS :
					pl_DonCrtSYS->ListeEquipement  = pl_Equipement;
					break;
				case XDC_EQT_LTN :
					pl_DonCrtLTN->ListeEquipement  = pl_Equipement;
					break;
			}
        }
    }
    
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Suppression structure de donnee Capteur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_sup_liste_Capteur ( ENIX_DONNEES_Capteur *  pa_DebCapteur )

/*
* ARGUMENTS EN ENTREE :
*
*   ENIX_DONNEES_Capteur **    pa_DebCapteur : Debut liste de Capteur a supprimer
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Liberation de la memoire pour toutes les structures de la liste
*
------------------------------------------------------*/
{
   static char *version = "@(#)enix_don.c	1.6 09/30/13      : et_sup_liste_Capteur " ;
   ENIX_DONNEES_Capteur    *    pl_ExCapt;
   ENIX_DONNEES_Capteur    *    pl_Capt;
 
 
   /*A pour chaque enregistrement de la liste */
   for ( pl_ExCapt = pl_Capt = pa_DebCapteur;
         pl_Capt != NULL; 
         pl_ExCapt = pl_Capt )
   {
      /* Liberation memoire */
      pl_Capt = pl_Capt->Suivant;
      free ( pl_ExCapt );
   }
   
   pa_DebCapteur = NULL;

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Suppression structure de donnee Sequence
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_sup_liste_Sequence ( ENIX_DONNEES_Sequence *    pa_DebSequence )

/*
* ARGUMENTS EN ENTREE :
*
*   ENIX_DONNEES_Sequence *    pa_DebSequence : Debut liste de Sequence a supprimer
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Liberation de la memoire pour toutes les structures de la liste
*
------------------------------------------------------*/
{
    static char                *version = "@(#)enix_don.c	1.6 09/30/13      : et_sup_liste_Sequence " ;
    ENIX_DONNEES_Sequence *    pl_ExSequence;
    ENIX_DONNEES_Sequence *    pl_Sequence;

    XZST_03EcritureTrace( XZSTC_INFO, "IN : et_sup_liste_Sequence");

    /*A pour chaque enregistrement de la liste */
    for ( pl_ExSequence = pl_Sequence = pa_DebSequence;
          pl_Sequence  != NULL; 
          pl_ExSequence = pl_Sequence )
    {
        /* Liberation memoire */
        pl_Sequence = pl_Sequence->Suivant;
        free ( pl_ExSequence );
    }
   
    pa_DebSequence = NULL;

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Suppression structure de donnee Equipement
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_sup_liste_Equipement ( ENIX_DONNEES_Equipement *    pa_DebEquipement )

/*
* ARGUMENTS EN ENTREE :
*
*   ENIX_DONNEES_Equipement *    pa_DebEquipement : Debut liste Equipement a supprimer
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Liberation de la memoire pour toutes les structures de la liste
*
------------------------------------------------------*/
{
    static char                *version = "@(#)enix_don.c	1.6 09/30/13      : et_sup_liste_Equipement " ;
    ENIX_DONNEES_Equipement *  pl_ExEquipement;
    ENIX_DONNEES_Equipement *  pl_Equipement;

    XZST_03EcritureTrace( XZSTC_INFO, "IN : et_sup_liste_Equipement");

    /*A pour chaque enregistrement de la liste */
    for ( pl_ExEquipement = pl_Equipement = pa_DebEquipement;
          pl_Equipement  != NULL; 
          pl_ExEquipement = pl_Equipement )
    {
        /* Liberation memoire */
        pl_Equipement = pl_Equipement->Suivant;
        free ( pl_ExEquipement );
    }

    pa_DebEquipement = NULL;

}

