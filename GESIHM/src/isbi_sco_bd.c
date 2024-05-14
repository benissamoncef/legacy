/*E*/
/*Fichier : $Id: isbi_sco_bd.c,v 1.2 2012/05/29 10:09:27 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2012/05/29 10:09:27 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE MSCO * FICHIER isyn_sco_bd.c
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de traitement des courbes :
*  sous-module d'interface avec la base de donnees
*  permettant la recherche des donnees RADT d'une periode
*  pour tous types de courbes F(t) (avec reference ou non)
*  en serialisant les requetes.
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	08 Nov 1994	: Creation
* LCL		20/12/21	: MOVIS P2 ems SAE-312
-----------------------------------------------------------------------------*/

/* fichiers inclus */

#include "isbi_sco_bd.h"
#include "isbi_sco.h"

/* definitions de constantes */

/* definitions de types locaux */
typedef struct {
               tm_caracteristique_courbe *caract;
               XDY_Octet fenetre;
               int no_courbe;
} tm_requete;


/* definition de macro locales */

/* declaration de variables locales */
static char *version = "@(#)isyn_sco_bd.c	1.17 09/18/07 : isyn_sco_bd" ;

static tm_requete vm_en_cours = { NULL, 0, 0 };
static tm_requete vm_en_attente = { NULL, 0, 0 };


static T_CB vm_callback[CM_NB_MAX_FENETRES] = { NULL, NULL };
static XDY_Datagroup vm_datagroup;

static int vm_nb_rqt_en_cours = 0;


extern int vm_TypeMachine;

/* declaration de fonctions internes */
void MSCO_traiter_donnees_bd_recues();

/* definition de fonctions externes */
extern XDY_Octet MSCO_Type_Courbe();
extern void MSCO_Recup_donnees_FT ();


/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Lancement effectif d'une requete en base de donnees
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static MSCO_lancer_requete(
                    pa_caract,
                    va_no_courbe)

                    tm_caracteristique_courbe *pa_caract;
                    int va_no_courbe;

/*
* ARGUMENTS EN ENTREE :
*  pa_caract : Caracteristiques de la courbe.
*  va_no_courbe : Numero de la courbe.
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   Chaque execution effective de requete
*
* FONCTION
*   Memoriser les caracteristiques de la requete en cours et la lancer
*
-----------------------------------------------------------------------------*/
{
   /*A Demander les donnees pour la courbe (la variable) de numero indique */
   vm_en_cours.caract = pa_caract;
   vm_en_cours.fenetre = pa_caract->fenetre;
   vm_en_cours.no_courbe = va_no_courbe;

   XZST_03EcritureTrace( XZSTC_WARNING, "Demande donnees %s : %d_%d_%d_%d %lf",
                           vm_datagroup ,
                           pa_caract->varcaract[va_no_courbe].no_station ,
                           pa_caract->varcaract[va_no_courbe].sens ,
                           pa_caract->varcaract[va_no_courbe].voie ,
                           pa_caract->varcaract[va_no_courbe].mesure,
			   pa_caract->horodeb);

   /* Si on est sur PC simplifie et station hors district alors on fait la demande au CI */
      XZAS06_Fonction_De_T( vm_datagroup ,
                           pa_caract->varcaract[va_no_courbe].no_station ,
                           pa_caract->varcaract[va_no_courbe].sens ,
                           pa_caract->varcaract[va_no_courbe].voie ,
                           pa_caract->horodeb,
                           pa_caract->varcaract[va_no_courbe].mesure );
   return XDC_OK;
}





/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Terminaison ou annulation d'une requete (en cours ou en attente)
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void MSCO_fin_requete(
                    pa_requete)

                    tm_requete *pa_requete;

/*
* ARGUMENTS EN ENTREE :
*   pa_requete : descriptif de la requete
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR :
*   Aucun
*
* CONDITION D'UTILISATION
*   Arrivee des donnees de resultat de requete ;
*   Annulation d'une requete
*
* FONCTION
*   Remise a zero des caracteristiques de la requete
*
-----------------------------------------------------------------------------*/
{
   pa_requete->caract = NULL;
   pa_requete->fenetre = 0;
   pa_requete->no_courbe = 0;
}





/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Requete en base de donnees pour obtenir les donnees d'une courbe F(t).
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_requete_courbe(
                    pa_caract,
                    va_no_courbe)

                    tm_caracteristique_courbe *pa_caract;
                    int va_no_courbe;

/*
* ARGUMENTS EN ENTREE :
*  pa_caract : Caracteristiques de la courbe.
*  va_no_courbe : Numero de la courbe.
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   Initialisation de la visualisation d'une courbe F(t),
*     avec reference ou non.
*
* FONCTION
*   Verifier qu'aucune requete (en cours ou mise en attente)
*   n'a deja ete faite pour la meme fenetre ;
*   si une requete est en cours pour l'autre fenetre alors
*   mettre la requete en attente sinon effectuer la demande de donnees.
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_bd.c	1.17 09/18/07 : MSCO_requete_courbe " ;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_requete_courbe" );

   /*A Verifier qu'une requete n'est pas en cours pour la meme fenetre */
   if ( ( vm_en_cours.caract != NULL )  &&  ( vm_en_cours.fenetre == pa_caract->fenetre ) )
      {
      XZST_03EcritureTrace( XZSTC_WARNING, "Une requete en base est deja en cours pour la fenetre %d",
                               pa_caract->fenetre);
      return XDC_NOK;
      }

   /*A Si une requete est en cours pour l'autre fenetre alors la mettre en attente */
   if ( ( vm_nb_rqt_en_cours > 0 )  &&  ( vm_callback[pa_caract->fenetre-1] == NULL ) )
      {
      if ( vm_en_attente.caract != NULL )
         {
         XZST_03EcritureTrace( XZSTC_WARNING, "Requete deja en attente pour fenetre %d, requete ignoree pour fenetre %d",
                                  vm_en_attente.fenetre, pa_caract->fenetre );
         return XDC_NOK;
         }
      else
         {
         vm_en_attente.caract = pa_caract;
         vm_en_attente.fenetre = pa_caract->fenetre;
         vm_en_attente.no_courbe = va_no_courbe;
         XZST_03EcritureTrace( XZSTC_WARNING, "Il existe une requete en cours. Requete courante mise en attente");
         return XDC_OK;
         }
      }

   /*A Mettre en place la recuperation des donnees (si ce n'est pas deja fait) */
   if ( vm_callback[pa_caract->fenetre-1] == NULL )
      {
         if ( MSCO_demarrer_recuperation (pa_caract) != XDC_OK )
            return XDC_NOK;
      }

   /*A Lancer la requete */
   MSCO_lancer_requete (pa_caract, va_no_courbe);

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_requete_courbe" );
   return XDC_OK;
}




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Demarrer la recuperation des donnees en base pour une courbe.
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_demarrer_recuperation(
                    pa_caract)

                    tm_caracteristique_courbe *pa_caract;


/*
* ARGUMENTS EN ENTREE :
*   pa_caract : Caracteristiques de la courbe.
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   Abonnement au datagroup correspondant aux donnees ;
*   Armement de la callback de recuperation.
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_bd.c	1.17 09/18/07 : MSCO_demarrer_recuperation " ;

   T_BOOL vl_etat;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_demarrer_recuperation" );

   /*A Si ce n'est deja fait */
   if ( vm_callback[pa_caract->fenetre-1] == NULL )
      {
      vm_nb_rqt_en_cours++;

      /*A Abonnement XDG_IDONNEES_FT+vm_NomMachine pour le callback */
      XDG_EncodeDG2( vm_datagroup , XDG_IDONNEES_FT , vm_NomMachine );
      TipcSrvDgGetRecv( vm_datagroup , &vl_etat );
      if ( vl_etat == FALSE )
         {
         if (! XDG_JoindreDG(vm_datagroup) )
            {
            vm_nb_rqt_en_cours--;
            XZST_03EcritureTrace( XZSTC_WARNING, "Abonnement sur %s non effectuee. \n " ,vm_datagroup);
            return XDC_NOK;
            }
         }

      /*A Armement Callback sur Reception XDM_IDONNEES_FT */
      vm_callback[pa_caract->fenetre-1] = TipcSrvProcessCbCreate( XDM_IdentMsg( XDM_IDONNEES_FT ),
                                                                  MSCO_traiter_donnees_bd_recues,
                                                                  (T_CB_ARG) pa_caract);
      if ( vm_callback[pa_caract->fenetre-1] == NULL)
         {
         vm_nb_rqt_en_cours--;
         XZST_03EcritureTrace( XZSTC_WARNING, "Callback sur %d non declenchee. \n ", XDM_IDONNEES_FT);
         return XDC_NOK;
         }
      }
   else
      {
      XZST_03EcritureTrace( XZSTC_WARNING, "Callback sur %d deja armee. \n ", XDM_IDONNEES_FT);
      /*return XDC_NOK;*/
      }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_demarrer_recuperation ");
   return XDC_OK;
}




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Arret de la recuperation de donnees pour une fenetre.
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_arret_recuperation(
		va_fenetre )

		XDY_Octet va_fenetre;

/*
* ARGUMENTS EN ENTREE :
*   va_fenetre : Numero de la fenetre.
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   Apres reception de toutes les donnees ;
*   Sur action "Quitter" ;
*   Sur reinitialisation du graphe d'une fenetre.
*
* FONCTION
*   Desarmement de la callback de reception des donnees pour la fenetre.
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_bd.c	1.17 09/18/07 : MSCO_arret_recuperation " ;

   tm_requete vl_en_attente;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_arret_recuperation" );

   /*A Si une requete etait en cours pour cette fenetre */
   if ( vm_callback[va_fenetre -1] != NULL )
      {
      MSCO_fin_requete (&vm_en_cours);

      /*A Desarmer la callback en place */
      if ( TutCbDestroy( vm_callback[va_fenetre -1] ) == FALSE )
         {
         XZST_03EcritureTrace( XZSTC_WARNING, "Desarmement de la Callback sur %d non effectuee ",
                                  XDM_IDONNEES_FT);
         return XDC_NOK;
         }
      else
         {
         vm_callback[va_fenetre -1] = NULL;
         }

      /*A Si une requete etait en attente alors la lancer */
      vl_en_attente = vm_en_attente;
      if ( vl_en_attente.caract != NULL )
         {
         if ( MSCO_demarrer_recuperation (vl_en_attente.caract) == XDC_OK )
            {
            MSCO_lancer_requete (vl_en_attente.caract, vl_en_attente.no_courbe);
            MSCO_fin_requete (&vm_en_attente);
            }
         }

      /*A Decompter la requete arretee */
      vm_nb_rqt_en_cours--;
      }
   /*A sinon annuler toute eventuelle requete en attente pour cette fenetre */
   else
      {
      if ( (vm_en_attente.caract != NULL)  &&  (vm_en_attente.fenetre == va_fenetre) )
         MSCO_fin_requete (&vm_en_attente);
      }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_arret_recuperation " );
   return XDC_OK;
}




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Traitement des donnees recues pour la requete en cours.
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSCO_traiter_donnees_bd_recues(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_caract)

/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une callback
*   pa_caract : caracteristiques de la courbe.
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR :
*   Aucun
*
* CONDITION D'UTILISATION
*   Sur reception de donnees
*
* FONCTION
*   Traitement specifique selon le type de graphe dans la fenetre
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_bd.c	1.17 09/18/07 : MSCO_traiter_donnees_bd_recues " ;

#ifdef _TIBCO_EMS
   XDY_Mot vl_Existe;
   XDY_Entier vl_NumStation;
   XDY_Mot vl_Sens,vl_Voie,vl_TypeDonnees;
   XDY_Horodate vl_HorodateDebut;
   XDY_Entier pl_valeur[XZISC_NB_LIG_MESURES200];
   XDY_Mot pl_validite[XZISC_NB_LIG_MESURES200];
   XDY_Entier vl_size_val;
   XDY_Entier vl_size_valid;
#else
   T_INT2 vl_Existe;
   T_INT4 vl_NumStation;
   T_INT2 vl_Sens,vl_Voie,vl_TypeDonnees;
   T_REAL8 vl_HorodateDebut;
   T_INT4 *pl_valeur;
   T_INT2 *pl_validite;
   T_INT4 vl_size_val;
   T_INT4 vl_size_valid;
#endif

   tm_caracteristique_courbe *pl_caract = (tm_caracteristique_courbe *) pa_caract;
   tm_requete vl_en_cours = vm_en_cours;
   XDY_Octet vl_type_courbe;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_traiter_donnees_bd_recues" );
XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: ICI avant TipcMsgRead %d",pl_caract->varcaract[vl_en_cours.no_courbe].mesure);

   /* Recuperation des donnees du message */
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT2,&vl_Existe,
                                T_IPC_FT_INT4,&vl_NumStation,
                                T_IPC_FT_INT2,&vl_Sens,
                                T_IPC_FT_INT2,&vl_Voie,
                                T_IPC_FT_REAL8,&vl_HorodateDebut,
                                T_IPC_FT_INT2,&vl_TypeDonnees,
                                T_IPC_FT_INT4_ARRAY,&pl_valeur,&vl_size_val,
                                T_IPC_FT_INT2_ARRAY,&pl_validite,&vl_size_valid, NULL) )
      {
      if ( ( vl_size_val != XZISC_NB_LIG_MESURES )  ||  ( vl_size_valid != XZISC_NB_LIG_MESURES ) )
         {
         XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: Le message recu n'a pas la bonne longueur de donnees pour les voies: %d %d",vl_size_val,vl_size_valid);
         return;
         }
      }
   else
      {
      XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: Impossible de recuperer le message");
      return;
      }
XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: ICI");
XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: ICI0 %d",vl_TypeDonnees);

   if ( vl_en_cours.caract == NULL )
      {
      XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: Requete en cours non definie !");
      return;
      }
XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: ICI00");
XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: ICI00 %d",vl_TypeDonnees);
XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: ICI00 %d",vl_en_cours.no_courbe);
XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: ICI00 %d",pl_caract->varcaract[vl_en_cours.no_courbe].mesure);

   if ( vl_TypeDonnees != pl_caract->varcaract[vl_en_cours.no_courbe].mesure )
      {
XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: ICI01");
      XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: Type de donnees attendu %d, recu %d",
                              pl_caract->varcaract[vl_en_cours.no_courbe].mesure, vl_TypeDonnees );
      return;
      }
XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: ICI00");


   /*A La requete en cours est consideree satisfaite */
   MSCO_fin_requete (&vm_en_cours);
XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: ICI1");


   /*A Traiter les donnees selon le type de courbe dans la fenetre */
   vl_type_courbe = MSCO_Type_Courbe (pl_caract->fenetre);
   if ( vl_type_courbe == CM_COURBE_FT )
      {
XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: ICI2");
      MSCO_Recup_donnees_FT (pl_caract, vl_Existe, vl_HorodateDebut, pl_valeur, pl_validite);
      }
   else if ( vl_type_courbe == CM_COURBE_FT_REF )
      {
XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: ICI3");
      MSCO_Recup_donnees_FT_Ref (pl_caract, vl_Existe, vl_HorodateDebut, pl_valeur, pl_validite);
      }
   else
      {
      XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_traiter_donnees_bd_recues: Pas de courbe F(t) dans la fenetre !");
      return;
      }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_traiter_donnees_bd_recues" );
   return ;
}
