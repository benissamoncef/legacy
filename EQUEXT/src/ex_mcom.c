/*E*/
/*Fichier : $Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $      Release : $Revision: 1.20 $        Date : $Date: 2021/05/04 13:25:03 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_mcom.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Definition des fonctions communes au Sous-Systeme EQUEXT
*
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	27 oct 1994	: Creation
* Mismer.D	version 1.2	28 Nov 1994	: Remplacement XZECT_ENG_CONF_LIGNE par XZEXT_ENG_CONF_LIGNE
* Mismer.D	version 1.3	06 Jan 1995	:
* Mismer.D	version 1.4	25 Jan 1995	:
* Mismer.D	version 1.5	26 Jan 1995	:
* Mismer.D	version 1.6	09 Mar 1995	:
* Fontaine.C	version 1.7	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.8	01 Sep 1995	: Modif fonction ecom_init pour ouverture ligne sans ouverture console
* Mismer.D  	version 1.9	07 Dec 1995	: Modif ouverture connexion transerver
* Mismer.D  	version 1.10	14 Dec 1995	: Remodif ouverture connexion transerver
* Mismer.D  	version 1.11	07 Fev 1996	: Ajout fonction  ecom_traitement_liaison_ST
* Mismer.D      version 1.12    30 Mai 1997     : Mise en place serveur de terminaux XYPLEX (DEM/14311
*
* JPL		23/02/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.13
* JMG		07/03/16 : ST 40 ports 1.14
* JMG		27/02/18 : IP CTRL_FEUX 1.15 DEM1284
* JMG		02/12/18 : Adaptation suppression SAGA DEM 1306 1.16
* JMG		13/05/19 : reprise file attente 1.17
*		07/11/19 : correction suppression commande cause manque initialisation horodate commande DEM1364 1.17
* jmg		15/01/21 : correction purge 1.19
* ABE		19/02/21 : Ajout fonction décodage trame OPC
-----------------------------------------------------*/

/* fichiers inclus */

#include "ex_msok.h"
#include "ex_mdon.h"
#include "xzex.h"

#include "ex_mcom.h"

/* definitions de constantes */
#define C_FILE_ST40 "/produits/migrazur/appliSD/fichiers/deq/ConfigST.txt"

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : elxi_common";

/* declaration de fonctions externes */

/* definition de fonctions internes */

int ex_st40(char *va_machine)
{

    int vl_Fd;
    char pl_PathEtFic[100];
    XDY_Texte pl_LigneFich;

    XZST_03EcritureTrace(XZSTC_FONCTION, "ex_st40: check pour machine %s", va_machine);
    sprintf(pl_PathEtFic, "%s/%s", XZEXC_PATH_CONFIG, XDF_ST40);
    if ((vl_Fd = open(pl_PathEtFic, O_RDONLY)) == -1)
    {
        XZST_03EcritureTrace(XZSTC_WARNING, " ex_st40 :  Fichier %s introuvable \n", pl_PathEtFic);
        return (XDC_NOK);
    }

    while (ex_LireLigneFich(vl_Fd, pl_LigneFich, sizeof(pl_LigneFich)) != XZEXC_EOF)
    {
        if (*pl_LigneFich != XDF_COMMENTAIRE)
        {
            XZST_03EcritureTrace(XZSTC_FONCTION, "%s", pl_LigneFich);
            if (!strcmp(va_machine, pl_LigneFich))
            {
                XZST_03EcritureTrace(XZSTC_FONCTION, "ex_st40: serveur de terminaux 40 ports");
                close(vl_Fd);
                return (XDC_OK);
            }
        }
    }
    close(vl_Fd);

    XZST_03EcritureTrace(XZSTC_FONCTION, "ex_st40: serveur de terminaux 20 ports");
    return (XDC_NOK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Lecture configuration de la ligne serie recu en argument.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* int ecom_lect_config_ligne_serie( XDY_NomMachine		va_NomMachine,
*				  XZEXT_ENG_CONF_LIGNE 		*pa_ConfigLigne,
*				  int				va_NoLigneSerie )
*
*
* ARGUMENTS EN ENTREE :
*   va_NomMachine	: Nom de la machine
*   va_NoLigneSerie	: No. de la ligne serie dont on lit la config.
*
* ARGUMENTS EN SORTIE :
*   pa_ConfigLigne	: pointeur sur structure stokage contenu configuration de la ligne serie
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*   lecture chaque ligne fichier config. pour trouver la config. de la ligne serie recue en par.
*   retour a la decouverte de la config.
------------------------------------------------------*/

int ecom_lect_config_ligne_serie(XDY_NomMachine va_NomMachine,
                                 XZEXT_ENG_CONF_LIGNE *pa_ConfigLigne,
                                 int va_NoLigneSerie)
{

    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_lect_config_ligne_serie";

    int vl_ValRet = XDC_OK; /* valeur de retour */
    int vl_Fd;              /* descripteur de fichier config */
    char *vl_Tmp, *vl_Tmp1;
    char pl_PathEtFic[XDC_PATH_ABS_SIZE]; /* chemin d'acces fichier */
    XDY_Texte pl_LigneFich;               /* buffer stokage une ligne de texte */
    XZEXT_ENG_CONF_LIGNE vl_ConfigTmp;    /* struct. stokage config. lue pour verif. */
    char vl_Str[16];

    /*A
     * Lecture du fichier de configuration XDF_Config_Lignes_<NomMachine>
     */

    /*A Construction du nom du fichier XDF_Config_Lignes_<NomMachine>  */
    sprintf(pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_Lignes, va_NomMachine);

    /*A Ouverture du fichier XDF_Config_Lignes_<NomMachine>  */
    if ((vl_Fd = open(pl_PathEtFic, O_RDONLY)) <= 0)
    {
        /*A Ecriture trace absence fichier */
        XZST_03EcritureTrace(XZSTC_WARNING, "ecom_lect_config_ligne_serie TELMi ligne %d : Absence fichier config. <%s>. ",
                             va_NoLigneSerie, pl_PathEtFic);
        /*A Sortie de la fonction */
        return (XDC_NOK);
    }

    /*A Tant que config ligne serie va_NoLigneSerie : lecture de chaque ligne du fichier */
    vl_ValRet = XDC_NOK;
    while (ex_LireLigneFich(vl_Fd, (char *)pl_LigneFich, sizeof(XDY_Texte)) != XZEXC_EOF)
    {
        /*A Lire ligne suivante si ligne courrante = commentaire */
        if (*pl_LigneFich != XDF_COMMENTAIRE)
        {
            /*B  Extraction config. definie sur la ligne texte courrante */
            sscanf(pl_LigneFich, "%s %s %c %c %s %d %d %d",
                   vl_ConfigTmp.NumeroST,
                   vl_ConfigTmp.NumLigne,
                   &(vl_ConfigTmp.Protocole),
                   &(vl_ConfigTmp.Sens),
                   vl_ConfigTmp.Vitesse,
                   &(vl_ConfigTmp.NbEssais),
                   &(vl_ConfigTmp.NbCarPref),
                   &(vl_ConfigTmp.NbCarSuff));

            /*A recuperation no. ligne */
            for (vl_Tmp = vl_ConfigTmp.NumLigne, vl_Tmp1 = vl_Str; *vl_Tmp != '_'; vl_Tmp++, vl_Tmp1++)
            {
                *vl_Tmp1 = *vl_Tmp;
            }
            *vl_Tmp1 = '\0';
            /*A SI NoLigneSerie trouve dans la derniere ligne de config. lue
             *  ALORS stokage config ds. pa_ConfigLigne et sortie du while de lecture.
             */
            if (atoi(vl_Str) == va_NoLigneSerie)
            {
                *pa_ConfigLigne = vl_ConfigTmp;
                vl_ValRet = XDC_OK;
                break;
            }
        }
    }
    /*A Fermeture du fichier  XDF_Config_Lignes_<NomMachine> */
    XZSS_13FermerFichier(vl_Fd);

    /*A SI probleme lecture config. ALORS trace Config ligne serie <no> absente ds. fichier <fic>" */
    if (vl_ValRet != XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING, "ecom_lect_config_ligne_serie TELMi ligne %d : Config ligne serie <%d> absente ds. fichier <%s>",
                             va_NoLigneSerie, va_NoLigneSerie, pl_PathEtFic);
    }

    /*A Retourne compte rendu */
    return (vl_ValRet);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Traitement de la connexion avec le serveur de terminaux
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*/
void ecom_traitement_liaison_ST(XZEXT_ST_Config *pa_Config,
                                XZEXT_MASK **pa_SockList,
                                XZEXT_ST_Sockets *pa_desSocks,
                                int *pa_EtatST)
/*
*
* ARGUMENTS EN ENTREE :
*
* pa_Config	: pointeur sur structure stokage config non initialisee.
* pa_SockList	: pointeur sur la liste des sockets.
* pa_desSocks	: pointeur sur les sockets specifiques taches TELxi.
*
* ARGUMENTS EN SORTIE :
* pa_Config	: pointeur sur structure config mise a jour
* pa_SockList	: pointeur sur la liste des sockets mise a jour
* pa_desSocks	: pointeur sur les sockets specifiques taches TELximises a jour.
*
*
* CODE RETOUR :
*
* NEANT
*
* CONDITION D'UTILISATION
*
* Appel de cette fonction si la liaison avec le serveur de terminaux est HS
*
* FONCTION
*
* Essai de connexion sur la ligne serie
*
------------------------------------------------------*/
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_traitement_liaison_ST  ";

    XDY_Texte vl_Texte = "";
    static int vl_CptTimeOut;
    static int vl_EtatLS = XDC_OK;

    if (vl_CptTimeOut++ > (XZEXC_DUREE_ATTENTE * 2))
    {
        vl_CptTimeOut = 0;

        if (*pa_EtatST == XDC_OK)
        {
            if (ex_cnx_test_ST(pa_Config->ligne_serie.NumeroST, pa_Config->no_port) == XDC_OK)
            {
                *pa_EtatST = XDC_NOK;
                if (pa_desSocks->lser > 0)
                {
                    ex_fin_cnx(pa_desSocks->lser, pa_SockList);
                    pa_desSocks->lser = XZEXC_SOCKET_NULL;
                }
            }
            XZST_03EcritureTrace(XZSTC_DEBUG2, "Etat de la liaison serie :%s", (*pa_EtatST == XDC_OK) ? "OK" : "NOK");
        }

        if (*pa_EtatST != XDC_OK)
        {
            /*A Essai connexion avec ST */
            vl_CptTimeOut = 0;
            *pa_EtatST = ecom_init(pa_Config, pa_SockList, pa_desSocks);
            sprintf(vl_Texte, "Liaison serie %d du Serveur de Terminaux %s %s !\0",
                    pa_Config->no_ligne,
                    pa_Config->ligne_serie.NumeroST,
                    (*pa_EtatST == XDC_OK) ? "retablie" : "coupee");
        }

        if (vl_EtatLS != *pa_EtatST)
        {
            /*A message pour l'admin. systeme XZST_10ArchiverMsgSyst() : Etat connexion ST */
            XZST_10ArchiverMsgSyst(0, vl_Texte, NULL);
            XZST_03EcritureTrace(XZSTC_WARNING, "%s", vl_Texte);
            vl_EtatLS = *pa_EtatST;
        }
    }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Affichage contnue d'une structure de stokage config. ligne
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void ecom_aff_config_ligne_serie	( XZEXT_ENG_CONF_LIGNE 	*pa_ConfigLigne )
*
*
* ARGUMENTS EN ENTREE :
*   pa_ConfigLigne	: pointeur sur structure stokage contenu config. de la ligne serie
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*   NEANT
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*
------------------------------------------------------*/
void ecom_aff_config_ligne_serie(XZEXT_ENG_CONF_LIGNE *pa_ConfigLigne)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_aff_config_ligne_serie";

    XDY_Texte LigneTexte;

    sprintf(LigneTexte, "ST:%s /NL:%s /P:%c /S:%c /V:%s /E:%d /PF:%d /SF:%d\n",
            pa_ConfigLigne->NumeroST,
            pa_ConfigLigne->NumLigne,
            pa_ConfigLigne->Protocole,
            pa_ConfigLigne->Sens,
            pa_ConfigLigne->Vitesse,
            pa_ConfigLigne->NbEssais,
            pa_ConfigLigne->NbCarPref,
            pa_ConfigLigne->NbCarSuff);

    XZST_03EcritureTrace(XZSTC_INFO, " Config. ligne serie : \n%s\n", LigneTexte);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Initialisation tache TELMi
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* int ecom_init( 	XZEXT_ST_Config *pa_Config,
*			XZEXT_MASK **pa_SockList,
*			XZEXT_ST_Sockets *pa_desSocks
            )
*
*
* ARGUMENTS EN ENTREE :
*
* pa_Config	: pointeur sur structure stokage config non initialisee.
* pa_SockList	: pointeur sur la liste des sockets.
* pa_desSocks	: pointeur sur les sockets specifiques taches TELxi.
*
* ARGUMENTS EN SORTIE :
* pa_Config	: pointeur sur structure config mise a jour
* pa_SockList	: pointeur sur la liste des sockets mise a jour
* pa_desSocks	: pointeur sur les sockets specifiques taches TELximises a jour.
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*   lecture fichier config. lignes pour recuperer ds. pa_ConfigLigne la config de la ligne geree
*   connexion en mode client sur la socket serveur des terminaux.
*
------------------------------------------------------*/
int ecom_init(XZEXT_ST_Config *pa_Config, XZEXT_MASK **pa_SockList, XZEXT_ST_Sockets *pa_DesSocks)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_init";

    XZEXT_MASK *vl_MasqueST = NULL; /* masque socket config. ligne geree */
    int vl_DesSock;                 /* socket comms. avec le ST par la console */
    char vl_CmdeST[512];            /* commande pour le ST */
    static int vl_CptEssai = 0;
    int vl_CodeRet = 0;

    XZST_03EcritureTrace(XZSTC_WARNING, "IN  : ecom_init ");

    /*A
     * lecture config. ligne serie geree.
     * remplissage donnees config. ds. la structure pa_ConfigLigne.
     */

    /*A SI lecture config. echoue, retour XDC_NOK */
    if (ecom_lect_config_ligne_serie(pa_Config->nom_machine,
                                     &(pa_Config->ligne_serie),
                                     pa_Config->no_ligne) != XDC_OK)
    {
        return (XDC_NOK);
    }
    /*A recuperation no port domaine internet */
    pa_Config->no_port = 2000 + (pa_Config->no_ligne * 100);
    /*A SI desc. socket lserie diff. de XZEXC_SOCKET_NULL ALORS fermeture socket */
    if (pa_DesSocks->lser != XZEXC_SOCKET_NULL)
    {
        ex_fin_cnx(pa_DesSocks->lser, pa_SockList);
        pa_DesSocks->lser = XZEXC_SOCKET_NULL;
    }

    /*A configuration de la BAUD RATE de la ligne geree */
    /*A SI ouverture console ST echoue */
    while (1)
    {
        if (ex_st40(pa_Config->nom_machine) == XDC_NOK)
            vl_CodeRet = ex_ouvrir_console_ST(pa_Config->ligne_serie.NumeroST, &vl_DesSock, &vl_MasqueST, pa_Config->nom_machine);
        else
            vl_CodeRet = ex_ouvrir_console_ST(pa_Config->ligne_serie.NumeroST, &vl_DesSock, pa_SockList, pa_Config->nom_machine);

        if (vl_CodeRet == XDC_OK)
            break;
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING, "ecom_init : Echec %deme essai connexion avec %s", vl_CptEssai, pa_Config->ligne_serie.NumeroST);
            if (vl_CptEssai++ > 1)
                break;
            sleep(10);
        }
    }

    if (vl_CodeRet == XDC_OK)
    {
        if (ex_st40(pa_Config->nom_machine) == XDC_OK)
            pa_DesSocks->lser = vl_DesSock;

        if (ex_st40(pa_Config->nom_machine) == XDC_NOK)
        {
            vl_CptEssai = 0;
            /*A formatage commande fermeture session transerver  */
            sprintf(vl_CmdeST, "KILL PORT %d SESSION ALL\r", pa_Config->no_ligne);
            if (ex_envoyer_commande_ST(vl_CmdeST, vl_DesSock, &vl_MasqueST) != XDC_OK)
            {
                /*A ALORS trace "ecom_init : Emission cmde. fermeture session transerver voie%d  impossible !!" */
                XZST_03EcritureTrace(XZSTC_WARNING, "ecom_init : Emission  cmde. fermeture session transerver voie%d  impossible !!!!",
                                     (pa_Config->no_ligne) - 1);
            }

            /*A
             * connexion soket en mode client pour les comms. avec le serveur des terminaux
             */

            XZST_03EcritureTrace(XZSTC_DEBUG1, "ecom_init : cnx. client %s, %d ",
                                 pa_Config->ligne_serie.NumeroST, pa_Config->no_port);

            sleep(1);

            /*A SI connexion echoue */
            if (ex_cnx_client_ST(pa_Config->ligne_serie.NumeroST,
                                 pa_Config->no_port,
                                 pa_SockList,
                                 &(pa_DesSocks->lser)) != XDC_OK)
            {
                /*A ALORS */
                /*B ecriture trace : " Connexion socket ligne serie mode client impossible !" */
                XZST_03EcritureTrace(XZSTC_WARNING,
                                     "ecom_init TELMi ligne %d : Connexion sur socket ligne serie %d ST <%s port %d> en mode client impossible !",
                                     pa_Config->no_ligne, pa_Config->no_ligne, pa_Config->ligne_serie.NumeroST, pa_Config->no_port);
                /*B mise XZEXC_SOCKET_NULL ds. pa_DesSocks->lser */
                pa_DesSocks->lser = XZEXC_SOCKET_NULL;

                /*A fermetur conexion avec la console ST */
                ex_fin_cnx(vl_DesSock, &vl_MasqueST);
                /*B retour ELxiC_ST_NOK */
                return (ELxiC_ST_NOK);
            }

            if ((atoi(pa_Config->ligne_serie.Vitesse) != 9600) && (atoi(pa_Config->ligne_serie.Vitesse) != 0))
            {
                /*A formatage commande ST config BAUD RATE : "set port x speed  yyyy\r" */
                sprintf(vl_CmdeST, "SET PO %d SPEED %s \r\0", pa_Config->no_ligne, pa_Config->ligne_serie.Vitesse);
                XZST_03EcritureTrace(XZSTC_WARNING, "Cmde ST BAUD RATE : %s", vl_CmdeST);

                /*A emission cmde config sur console ST */
                /*A SI emission cmde console ST echoue */
                if (ex_envoyer_commande_ST(vl_CmdeST, vl_DesSock, &vl_MasqueST) != XDC_OK)
                {
                    /*A ALORS trace "ecom_init TELMi ligne %d : Emission cmde. config. vitesse %s ligne %d sur console ST %s impossible !!" */
                    XZST_03EcritureTrace(XZSTC_WARNING, "ecom_init TELMi ligne %d : Emission cmde. config. vitesse %s ligne %d sur console ST %s impossible !!",
                                         pa_Config->no_ligne, pa_Config->ligne_serie.Vitesse, pa_Config->no_ligne, pa_Config->ligne_serie.NumeroST);
                    /*A fermetur conexion avec la console ST */
                    ex_fin_cnx(vl_DesSock, &vl_MasqueST);
                    /*A retour  ELxiC_ST_NOK */
                    return (ELxiC_ST_NOK);
                }
            }

            /*A fermetur conexion avec la console ST */
            ex_fin_cnx(vl_DesSock, &vl_MasqueST);
        }
    }
    else
    {
        /*A ALORS trace "Ouverture console ST impossible !!" */
        XZST_03EcritureTrace(XZSTC_WARNING, "ecom_init TELMi ligne %d : Ouverture console ST %s pour config. ligne serie %d impossible",
                             pa_Config->no_ligne, pa_Config->ligne_serie.NumeroST, pa_Config->no_ligne);
        if (ex_st40(pa_Config->nom_machine) == XDC_OK)
        {
            sprintf(vl_CmdeST, "/produits/migrazur/appliSD/exec/ConfigSTTelnet1Port.sh %s %d %d", pa_Config->ligne_serie.NumeroST, pa_Config->no_ligne,
                    atoi(pa_Config->ligne_serie.Vitesse));
            XZST_03EcritureTrace(XZSTC_WARNING, "%s", vl_CmdeST);
            system(vl_CmdeST);
        }
        return (ELxiC_ST_NOK);
    }

    /*A retour XDC_OK */
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : ecom_init ");
    return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Recherche adresse client selon soket active ds. la liste des sockets
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*  int ecom_detecte_clnt( XZEXT_ST_Sockets *pa_DesSocks, XZEXT_MASK **pa_SockList )
*
* ARGUMENTS EN ENTREE :
*
* pa_DesSocks	: pointeur sur les descs. sockets specifiques tache TELMi.
* pa_SockLis	: pointeur sur liste sockets.
*
* ARGUMENTS EN SORTIE :
* NEANT
*
* CODE RETOUR :
*
* XDC_NOK si la liste contient seulement XZEXC_NBR_MIN_SOCK_TELxi ou moins.
*
* CONDITION D'UTILISATION
*
* FONCTION
*
* - recherche ds. la liste des sockets l'adresse de la socket active et
*   la stoke ds. le champ pa_DesSocks->curr_addr.
*
------------------------------------------------------*/
int ecom_detecte_clnt(XZEXT_ST_Sockets *pa_DesSocks, XZEXT_MASK **pa_SockList)
{

    XZEXT_MASK *pl_List; /* pointeur qui "glisse" sur la liste */
    int i;               /* compteur(s) nbre. elems. liste */

    pa_DesSocks->curr_addr[0] = '\0';

    /*A
     * Recherche ds. la liste des sockets
     */
    /*A POUR toute la liste de sockets */
    for (i = 0, pl_List = *pa_SockList; pl_List != NULL; pl_List = pl_List->Suivant, i++)
    {
        /*B SI socket active trouvee ds. la liste ALORS mise a jour adresse courante */
        if (pl_List->desc_sock == pa_DesSocks->curr)
        {
            strncpy(pa_DesSocks->curr_addr, pl_List->adresse, XDC_PATH_ABS_SIZE - 1);
        }
    }

    /*A SI nbre. elems <= XZEXC_NBR_MIN_SOCK_TELxi retour XDC_NOK */
    if (i <= XZEXC_NBR_MIN_SOCK_TELxi)
        return (XDC_NOK);

    /*A SINON retour XDC_OK */
    return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* colle une cmde. LCR a la fin de la liste des cmdes. LCR pointee par pa_List.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*  void ecom_append_cmd_LCR( XZEXT_Cmde_LCR *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR **pa_DebListe )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. LCR a inserer ds. la liste.
* pa_DebListe	: pointeur sur une liste chainee de cmdes. LCR
*
* ARGUMENTS EN SORTIE :
* pa_DebListe	: la liste mise a jour avec le nouvel element.
*
* CODE RETOUR :
*
* NEANT
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*
*
------------------------------------------------------*/
void ecom_append_cmd_LCR(XZEXT_Cmde_LCR *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR **pa_DebListe)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_append_cmd_LCR";

    XZEXT_Liste_Cmdes_LCR *pl_Elm;     /* pointeur sur l'elem. courant de la liste */
    XZEXT_Liste_Cmdes_LCR *pl_ElmPrec; /* pointeur sur l'elem. precedent ds. la liste */
    XZEXT_Liste_Cmdes_LCR *pl_NewElm;  /* pointeur sur l'elem a inserer */

    /*A SI espace memoire disponible ALORS ajout element */
    if (pl_NewElm = (XZEXT_Liste_Cmdes_LCR *)malloc(sizeof(XZEXT_Liste_Cmdes_LCR)))
    {
        /*A stokage cmde. ds. le nouvel futur elem. de la liste */
        pl_NewElm->cmd = *pa_CmdeLCR;

        /*A SI la liste etait vide ALORS "ajout" nouvel elem. en tete de liste RETURN */
        if (*pa_DebListe == NULL)
        {
            /*B cas liste vide avant ajout nouvel elem */
            pl_NewElm->next = *pa_DebListe;
            *pa_DebListe = pl_NewElm;
            return;
        }

        /*A SINON pour la liste entiere recherche fin de la liste */
        for (pl_Elm = pl_ElmPrec = *pa_DebListe; pl_Elm != NULL;
             pl_ElmPrec = pl_Elm, pl_Elm = pl_Elm->next)
            ;

        /*A pl_ElmPrec pointe maintenant sur le dernier elem. de la liste */
        pl_NewElm->next = pl_Elm;
        pl_ElmPrec->next = pl_NewElm;
    }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* colle une cmde. LCR IP a la fin de la liste des cmdes. LCR IP pointee par pa_List.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*  void ecom_append_cmd_LCR_IP( XZEXT_Cmde_LCR_IP *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR_IP **pa_DebListe )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. LCR a inserer ds. la liste.
* pa_DebListe	: pointeur sur une liste chainee de cmdes. LCR
*
* ARGUMENTS EN SORTIE :
* pa_DebListe	: la liste mise a jour avec le nouvel element.
*
* CODE RETOUR :
*
* NEANT
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*
*
------------------------------------------------------*/
void ecom_append_cmd_LCR_IP(XZEXT_Cmde_LCR_IP *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR_IP **pa_DebListe)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_append_cmd_LCR";

    XZEXT_Liste_Cmdes_LCR_IP *pl_Elm;     /* pointeur sur l'elem. courant de la liste */
    XZEXT_Liste_Cmdes_LCR_IP *pl_ElmPrec; /* pointeur sur l'elem. precedent ds. la liste */
    XZEXT_Liste_Cmdes_LCR_IP *pl_NewElm;  /* pointeur sur l'elem a inserer */

    /*A SI espace memoire disponible ALORS ajout element */
    if (pl_NewElm = (XZEXT_Liste_Cmdes_LCR_IP *)malloc(sizeof(XZEXT_Liste_Cmdes_LCR_IP)))
    {
        /*A stokage cmde. ds. le nouvel futur elem. de la liste */
        pl_NewElm->cmd = *pa_CmdeLCR;

        /*A SI la liste etait vide ALORS "ajout" nouvel elem. en tete de liste RETURN */
        if (*pa_DebListe == NULL)
        {
            /*B cas liste vide avant ajout nouvel elem */
            pl_NewElm->next = *pa_DebListe;
            *pa_DebListe = pl_NewElm;
            XZST_03EcritureTrace(XZSTC_FONCTION, "append sur liste vide");
            return;
        }

        /*A SINON pour la liste entiere recherche fin de la liste */
        for (pl_Elm = pl_ElmPrec = *pa_DebListe; pl_Elm != NULL;
             pl_ElmPrec = pl_Elm, pl_Elm = pl_Elm->next)
            ;

        /*A pl_ElmPrec pointe maintenant sur le dernier elem. de la liste */
        pl_NewElm->next = pl_Elm;
        pl_ElmPrec->next = pl_NewElm;
        XZST_03EcritureTrace(XZSTC_FONCTION, "append fin de liste");
    }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
* Recuperation premier elem. de la liste des cmdes. LCR.
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void	ecom_getfirst_cmd_LCR( XZEXT_Cmde_LCR *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR **pa_DebListe )
*
* ARGUMENTS EN ENTREE :
*
* pa_DebListe 	: pointeur sur le debut de la liste.
*
* ARGUMENTS EN SORTIE :
*
* pa_DebListe	: pointeur sur le debut de la liste mise a jour
* pa_CmdeLCR	: cmde. LCR extraite .
*
* CODE RETOUR :
*
* NEANT.
*
* CONDITION D'UTILISATION
*
* liste non vide : a tester avant appel de cette fonction.
*
* FONCTION
*
* elimine le 1er elem. de la liste de cmdes. LCR et stoke l'info. utile ds. pa_CmdeLCR.
*
*
------------------------------------------------------*/
void ecom_getfirst_cmd_LCR(XZEXT_Cmde_LCR *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR **pa_DebListe)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_getfirst_cmd_LCR";

    XZEXT_Liste_Cmdes_LCR *pl_Elm; /* pointeur sur l'elem. courant de la liste */

    /*A extraction 1er elem. */
    pl_Elm = *pa_DebListe;
    *pa_CmdeLCR = pl_Elm->cmd;
    *pa_DebListe = pl_Elm->next;
    free(pl_Elm);
}

int ecom_purge_cmd(XDY_AdresseIP va_adresse, XDY_PortIP va_port, XZEXT_Liste_Cmdes_LCR_IP **pa_DebListe,
                   XZEXT_Cmde_LCR_IP *pa_cmd_sortie)
{
    XDY_Horodate vl_horodate;
    int vl_js;
    int vl_purge;
    XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_js, &vl_horodate);

    XZEXT_Liste_Cmdes_LCR_IP *pl_Elm;

    vl_purge = 0;
    pl_Elm = *pa_DebListe;
    while (pl_Elm != NULL)
    {
        /*on ne purge que sur l horodate*/
        /*if  ((pl_Elm->cmd.port==va_port) &&
                                 !strcmp(pl_Elm->cmd.adresseIP,va_adresse)){*/
        if (pl_Elm->cmd.horodate < vl_horodate - 59 /*600*/)
        {
            XZST_03EcritureTrace(XZSTC_WARNING, "purge : cmd purgee, trop ancienne");
            if (pl_Elm == *pa_DebListe)
                *pa_DebListe = (*pa_DebListe)->next;
            *pa_cmd_sortie = pl_Elm->cmd;
            free(pl_Elm);
            return (1);
            /*pl_Elm=*pa_DebListe;
            vl_purge =1 +   ecom_purge_cmd(va_adresse,va_port,pa_DebListe);*/
            /*ecom_delete_cmd_LCR_IP(va_adresse, va_port, pa_DebListe);     */
        }
        /*              }*/
        pl_Elm = pl_Elm->next;
    }
    XZST_03EcritureTrace(XZSTC_WARNING, "purge : retour %d", vl_purge);
    return (vl_purge);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
* Recuperation premier elem. de la liste des cmdes. LCR. IP
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void	ecom_getfirst_cmd_LCR_IP( XZEXT_Cmde_LCR_IP *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR_IP **pa_DebListe,
            XDY_AdresseIP va_adresse, XDY_PortIP va_port)
*
* ARGUMENTS EN ENTREE :
*
* pa_DebListe 	: pointeur sur le debut de la liste.
*
* ARGUMENTS EN SORTIE :
*
* pa_DebListe	: pointeur sur le debut de la liste mise a jour
* pa_CmdeLCR	: cmde. LCR extraite .
*
* CODE RETOUR :
*
* NEANT.
*
* CONDITION D'UTILISATION
*
* liste non vide : a tester avant appel de cette fonction.
*
* FONCTION
*
* elimine le 1er elem. de la liste de cmdes. LCR et stoke l'info. utile ds. pa_CmdeLCR.
*
*
------------------------------------------------------*/
XDY_Octet ecom_getfirst_cmd_LCR_IP(XZEXT_Cmde_LCR_IP *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR_IP **pa_DebListe,
                                   XDY_AdresseIP va_ip, XDY_PortIP va_port)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_getfirst_cmd_LCR";

    XZEXT_Liste_Cmdes_LCR_IP *pl_Elm; /* pointeur sur l'elem. courant de la liste */

    /*A extraction 1er elem. */
    pl_Elm = *pa_DebListe;
    while (pl_Elm != NULL)
    {
        if ((pl_Elm->cmd.port == va_port) &&
            !strcmp(pl_Elm->cmd.adresseIP, va_ip))
        {
            XZST_03EcritureTrace(XZSTC_WARNING, "cmd trouvee");
            *pa_CmdeLCR = pl_Elm->cmd;
            /*	*pa_DebListe = pl_Elm->next;
                free( pl_Elm );*/
            return (XDC_OK);
        }
        pl_Elm = pl_Elm->next;
    }
    pa_CmdeLCR = NULL;
    return (XDC_NOK);
}
XDY_Octet ecom_getfirst_cmd_LCR_IP2(XZEXT_Cmde_LCR_IP *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR_IP **pa_DebListe,
                                    char *va_ip)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_getfirst_cmd_LCR";

    XZEXT_Liste_Cmdes_LCR_IP *pl_Elm; /* pointeur sur l'elem. courant de la liste */

    /*A extraction 1er elem. */
    XZST_03EcritureTrace(XZSTC_WARNING, "IN delete");
    pl_Elm = *pa_DebListe;
    XZST_03EcritureTrace(XZSTC_WARNING, "2 delete");
    while (pl_Elm != NULL)
    {
        if (!strcmp(pl_Elm->cmd.adresseIP, va_ip))
        {
            XZST_03EcritureTrace(XZSTC_WARNING, "avant delete");
            *pa_CmdeLCR = pl_Elm->cmd;
            ecom_delete_cmd_LCR_IP(pl_Elm->cmd.adresseIP, pl_Elm->cmd.port,
                                   pa_DebListe);
            XZST_03EcritureTrace(XZSTC_WARNING, "apres delete");
            return (XDC_OK);
        }
        pl_Elm = pl_Elm->next;
        XZST_03EcritureTrace(XZSTC_WARNING, "apres next");
    }
    pa_CmdeLCR = NULL;
    XZST_03EcritureTrace(XZSTC_WARNING, "ecom_getfirst_cmd_LCR_IP2 : OUT");
    return (XDC_NOK);
}

int ecom_existe_cmd_LCR_IP(XDY_AdresseIP va_ip, XDY_PortIP va_port,
                           XZEXT_Liste_Cmdes_LCR_IP *pa_DebListe,
                           XZEXT_Cmde_LCR_IP *pa_cmd)
{
    XZEXT_Liste_Cmdes_LCR_IP *pl_Elm = NULL;
    int vl_js;
    XDY_Horodate vl_horodate;
    pl_Elm = pa_DebListe;

    XZST_03EcritureTrace(XZSTC_WARNING, "ip %s port %d",
                         va_ip, va_port);
    while (pl_Elm != NULL)
    {
        XZST_03EcritureTrace(XZSTC_WARNING, "cmd %s %d", pl_Elm->cmd.adresseIP, pl_Elm->cmd.port);
        if ((pl_Elm->cmd.port == va_port) &&
            !strcmp(pl_Elm->cmd.adresseIP, va_ip))
        {
            *pa_cmd = pl_Elm->cmd;
            return (XDC_OK);
        }
        pl_Elm = pl_Elm->next;
    }
    return (XDC_NOK);
}

void ecom_delete_cmd_LCR_IP(XDY_AdresseIP va_ip, XDY_PortIP va_port,
                            XZEXT_Liste_Cmdes_LCR_IP **pa_DebListe)
{
    XZEXT_Liste_Cmdes_LCR_IP *pl_Elm = NULL;
    XZEXT_Liste_Cmdes_LCR_IP *pl_Elm2 = NULL;
    XZST_03EcritureTrace(XZSTC_FONCTION, "ecom_delete_cmd_LCR_IP: %s %d",
                         va_ip, va_port);
    pl_Elm = *pa_DebListe;
    pl_Elm2 = *pa_DebListe;
    if (pl_Elm == NULL)
        return;

    XZST_03EcritureTrace(XZSTC_FONCTION, "cmd lue %s %d",
                         pl_Elm->cmd.adresseIP, pl_Elm->cmd.port);
    if ((pl_Elm->cmd.port == va_port) &&
        !strcmp(pl_Elm->cmd.adresseIP, va_ip))
    {
        XZST_03EcritureTrace(XZSTC_FONCTION, "cmd suivante");
        *pa_DebListe = pl_Elm->next;
        free(pl_Elm);
        return;
    }
    while (pl_Elm->next != NULL)
    {
        pl_Elm2 = pl_Elm->next;
        if ((pl_Elm2->cmd.port == va_port) &&
            !strcmp(pl_Elm2->cmd.adresseIP, va_ip))
        {
            pl_Elm->next = pl_Elm2->next;
            free(pl_Elm2);
            return;
        }
        pl_Elm = pl_Elm->next;
    }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* insertion d'une cmde. LCR ds. la liste selon la priorite attachee a la commande.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void ecom_insert_cmd_LCR( XZEXT_Cmde_LCR *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR **pa_DebListe )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. LCR a inserer ds. la liste.
* pa_DebListe	: pointeur sur une liste chainee de cmdes. LCR
*
* ARGUMENTS EN SORTIE :
* pa_DebListe	: la liste mise a jour avec le nouvel element.
*
* CODE RETOUR :
*
* NEANT
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*
*
------------------------------------------------------*/
void ecom_insert_cmd_LCR(XZEXT_Cmde_LCR *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR **pa_DebListe)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_insert_cmd_LCR";

    XZEXT_Liste_Cmdes_LCR *pl_Elm;     /* pointeur sur l'elem. courant de la liste */
    XZEXT_Liste_Cmdes_LCR *pl_ElmPrec; /* pointeur sur l'elem. precedent ds. la liste */
    XZEXT_Liste_Cmdes_LCR *pl_NewElm;  /* pointeur sur l'elem a inserer */

    /*A SI espace memoire disponible ALORS ajout element */
    if (pl_NewElm = (XZEXT_Liste_Cmdes_LCR *)malloc(sizeof(XZEXT_Liste_Cmdes_LCR)))
    {
        /*A stokage cmde. ds. le nouvel futur elem. de la liste */
        pl_NewElm->cmd = *pa_CmdeLCR;

        /*A SI la liste etait vide ALORS "ajout" nouvel elem. en tete de liste RETURN */
        if (*pa_DebListe == NULL)
        {
            /*B cas liste vide avant ajout nouvel elem */
            pl_NewElm->next = *pa_DebListe;
            *pa_DebListe = pl_NewElm;
            return;
        }

        /*A SINON pour la liste entiere recherche premier elem. de PRIORITE FAIBLE */
        for (pl_Elm = pl_ElmPrec = *pa_DebListe;
             pl_Elm != NULL && pl_Elm->cmd.prior != XZEXC_PRIORITEFAIBLE;
             pl_ElmPrec = pl_Elm, pl_Elm = pl_Elm->next)
            ;

        /*A SI cmde. a inserer de priorite FAIBLE */
        if (pl_NewElm->cmd.prior == XZEXC_PRIORITEFAIBLE)
        {
            /*A ALORS coller commande a la fin de la liste et RETOUR */
            ecom_append_cmd_LCR(pa_CmdeLCR, pa_DebListe);
            free(pl_NewElm);
            return;
        }

        /*A pl_ElmPrec pointe maintenant sur la derniere cmde. de forte priorite ds. la liste */
        /*A INSERTION de la cmde. */
        pl_NewElm->next = pl_Elm;
        pl_ElmPrec->next = pl_NewElm;
    }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* insertion d'une cmde. LCR  IP ds. la liste selon la priorite attachee a la commande.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void ecom_insert_cmd_LCR_IP( XZEXT_Cmde_LCR_IP *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR_IP **pa_DebListe )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. LCR a inserer ds. la liste.
* pa_DebListe	: pointeur sur une liste chainee de cmdes. LCR
*
* ARGUMENTS EN SORTIE :
* pa_DebListe	: la liste mise a jour avec le nouvel element.
*
* CODE RETOUR :
*
* NEANT
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*
*
------------------------------------------------------*/
void ecom_insert_cmd_LCR_IP(XZEXT_Cmde_LCR_IP *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR_IP **pa_DebListe)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_insert_cmd_LCR";

    XZEXT_Liste_Cmdes_LCR_IP *pl_Elm;     /* pointeur sur l'elem. courant de la liste */
    XZEXT_Liste_Cmdes_LCR_IP *pl_ElmPrec; /* pointeur sur l'elem. precedent ds. la liste */
    XZEXT_Liste_Cmdes_LCR_IP *pl_NewElm;  /* pointeur sur l'elem a inserer */
    XDY_Horodate vl_horodate;
    int vl_js;
    XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_js, &vl_horodate);

    XZST_03EcritureTrace(XZSTC_WARNING, " ecom_insert_cmd_LCR_IP : in %s %d %d",
                         pa_CmdeLCR->adresseIP,
                         pa_CmdeLCR->port,
                         pa_CmdeLCR->type_msg);
    /*A SI espace memoire disponible ALORS ajout element */
    if (pl_NewElm = (XZEXT_Liste_Cmdes_LCR_IP *)malloc(sizeof(XZEXT_Liste_Cmdes_LCR_IP)))
    {
        XZST_03EcritureTrace(XZSTC_WARNING, " apres malloc");
        /*A stokage cmde. ds. le nouvel futur elem. de la liste */
        pl_NewElm->cmd = *pa_CmdeLCR;
        pl_NewElm->cmd.horodate = vl_horodate;

        /*A SI la liste etait vide ALORS "ajout" nouvel elem. en tete de liste RETURN */
        if (*pa_DebListe == NULL)
        {
            /*B cas liste vide avant ajout nouvel elem */
            pl_NewElm->next = *pa_DebListe;
            *pa_DebListe = pl_NewElm;
            XZST_03EcritureTrace(XZSTC_WARNING, " avant return");
            return;
        }

        XZST_03EcritureTrace(XZSTC_WARNING, " ici");
        /*A SINON pour la liste entiere recherche premier elem. de PRIORITE FAIBLE */
        for (pl_Elm = pl_ElmPrec = *pa_DebListe;
             pl_Elm != NULL && pl_Elm->cmd.prior != XZEXC_PRIORITEFAIBLE;
             pl_ElmPrec = pl_Elm, pl_Elm = pl_Elm->next)
            ;

        /*A SI cmde. a inserer de priorite FAIBLE */
        if (pl_NewElm->cmd.prior == XZEXC_PRIORITEFAIBLE)
        {
            /*A ALORS coller commande a la fin de la liste et RETOUR */
            pa_CmdeLCR->horodate = vl_horodate;
            ecom_append_cmd_LCR_IP(pa_CmdeLCR, pa_DebListe);
            free(pl_NewElm);
            return;
        }

        /*A pl_ElmPrec pointe maintenant sur la derniere cmde. de forte priorite ds. la liste */
        /*A INSERTION de la cmde. */
        pl_NewElm->next = pl_Elm;
        pl_ElmPrec->next = pl_NewElm;
    }
    else
        XZST_03EcritureTrace(XZSTC_WARNING, "pb MALLOC");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* insertion d'une cmde. LCR en debut de la liste.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void ecom_insertfirst_cmd_LCR( XZEXT_Cmde_LCR *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR **pa_DebListe )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. LCR a inserer ds. la liste.
* pa_DebListe	: pointeur sur une liste chainee de cmdes. LCR
*
* ARGUMENTS EN SORTIE :
* pa_DebListe	: la liste mise a jour avec le nouvel element.
*
* CODE RETOUR :
*
* NEANT
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*
*
------------------------------------------------------*/
void ecom_insertfirst_cmd_LCR(XZEXT_Cmde_LCR *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR **pa_DebListe)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_insertfirst_cmd_LCR";

    XZEXT_Liste_Cmdes_LCR *pl_Elm;     /* pointeur sur l'elem. courant de la liste */
    XZEXT_Liste_Cmdes_LCR *pl_ElmPrec; /* pointeur sur l'elem. precedent ds. la liste */
    XZEXT_Liste_Cmdes_LCR *pl_NewElm;  /* pointeur sur l'elem a inserer */

    /*A SI espace memoire disponible ALORS ajout element */
    if (pl_NewElm = (XZEXT_Liste_Cmdes_LCR *)malloc(sizeof(XZEXT_Liste_Cmdes_LCR)))
    {
        /*A stokage cmde. ds. le nouveau futur elem. de la liste */
        pl_NewElm->cmd = *pa_CmdeLCR;

        /*A "ajout" nouvel elem. en tete de liste RETURN */
        pl_NewElm->next = *pa_DebListe;
        *pa_DebListe = pl_NewElm;
    }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* insertion d'une cmde. LCR IP en debut de la liste.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void ecom_insertfirst_cmd_LCR_IP( XZEXT_Cmde_LCR_IP *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR_IP **pa_DebListe )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. LCR a inserer ds. la liste.
* pa_DebListe	: pointeur sur une liste chainee de cmdes. LCR
*
* ARGUMENTS EN SORTIE :
* pa_DebListe	: la liste mise a jour avec le nouvel element.
*
* CODE RETOUR :
*
* NEANT
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*
*
------------------------------------------------------*/
void ecom_insertfirst_cmd_LCR_IP(XZEXT_Cmde_LCR_IP *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR_IP **pa_DebListe)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_insertfirst_cmd_LCR";

    XZEXT_Liste_Cmdes_LCR_IP *pl_Elm;     /* pointeur sur l'elem. courant de la liste */
    XZEXT_Liste_Cmdes_LCR_IP *pl_ElmPrec; /* pointeur sur l'elem. precedent ds. la liste */
    XZEXT_Liste_Cmdes_LCR_IP *pl_NewElm;  /* pointeur sur l'elem a inserer */

    /*A SI espace memoire disponible ALORS ajout element */
    if (pl_NewElm = (XZEXT_Liste_Cmdes_LCR_IP *)malloc(sizeof(XZEXT_Liste_Cmdes_LCR_IP)))
    {
        /*A stokage cmde. ds. le nouveau futur elem. de la liste */
        pl_NewElm->cmd = *pa_CmdeLCR;

        /*A "ajout" nouvel elem. en tete de liste RETURN */
        pl_NewElm->next = *pa_DebListe;
        *pa_DebListe = pl_NewElm;
    }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Lecture horodate systeme.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* char  *ecom_lire_horodate( XZSMT_Horodate *pa_Horodate );
*
*
* ARGUMENTS EN ENTREE :
*   pa_Horodate	: pointeur su horodate en format converti.
*
* ARGUMENTS EN SORTIE :
*   NEANT.
*
* CODE RETOUR :
*   pointeur sur horodate convetie en format jj/mm/aaaa hh:mm:ss si tout OK.
*   pointeur sur chaine "?" si la lecture de l'horodate s'est mal pasee.
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*   lecture chaque ligne fichier config. pour trouver la config. de la ligne serie recue en par.
*   retour a la decouverte de la config.
------------------------------------------------------*/

char *ecom_lire_horodate(XZSMT_Horodate *pa_Horodate)
{

    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_lire_horodate";

    XDY_Horodate vl_ValHorodate; /* stokage horodate valeur non convertie */
    int vl_JourSem;              /* jour de la semaine */
                                 /*
                                     vl_Horodate = (XZSMT_Horodate *)malloc( sizeof(XZSMT_Horodate) );
                                 */
    /*A stokage valeur horodate et conversion */

    /*A SI stockage val. horodate OK */
    if (XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_ValHorodate) == XZSMC_OK)
    {
        /*B ALORS SI conversion not OK ALORS horodate = "?" */
        if (XZSM_11ConversionHorodate(vl_ValHorodate, pa_Horodate) != XZSMC_OK)
        {
            strcpy(*pa_Horodate, "?");
        }
    }
    /*A SINON horodate = "?" */
    else
    {
        strcpy(*pa_Horodate, "?");
    }

    return ((char *)*pa_Horodate);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Destruction liste commandes LCR.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void	ecom_destroy_list_cmd_LCR( XZEXT_Liste_Cmdes_LCR **pa_DebListe );
*
*
* ARGUMENTS EN ENTREE :
*   pa_DebListe	: pointeur sur le debut de la liste de commandes.
*
* ARGUMENTS EN SORTIE :
*   NEANT.
*
* CODE RETOUR :
*   NEANT.
* CONDITION D'UTILISATION
*
*
* FONCTION
*   eliberation de la memoire ocuppee par la liste de cmdes. lCR non traitees.
*
------------------------------------------------------*/

void ecom_destroy_list_cmd_LCR(XZEXT_Liste_Cmdes_LCR **pa_DebListe)

{

    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_destroy_list_cmd_LCR";

    XZEXT_Liste_Cmdes_LCR *pl_Elm; /* pointeur sur l'elem. courant de la liste */

    /*A eliberation memoire pour tous les elems. de la liste */
    /*A TANT QUE elkem non NULL */
    pl_Elm = *pa_DebListe;
    while (pl_Elm != NULL)
    {
        *pa_DebListe = pl_Elm->next;
        free(pl_Elm);
        pl_Elm = *pa_DebListe;
    }
}
void ecom_destroy_list_cmd_LCR_IP(XZEXT_Liste_Cmdes_LCR_IP **pa_DebListe)

{

    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_destroy_list_cmd_LCR";

    XZEXT_Liste_Cmdes_LCR_IP *pl_Elm; /* pointeur sur l'elem. courant de la liste */

    /*A eliberation memoire pour tous les elems. de la liste */
    /*A TANT QUE elkem non NULL */
    pl_Elm = *pa_DebListe;
    while (pl_Elm != NULL)
    {
        *pa_DebListe = pl_Elm->next;
        free(pl_Elm);
        pl_Elm = *pa_DebListe;
    }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Destruction liste de sockets.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void	ecom_detruire_liste_sock( XZEXT_MASK **pa_DebListe );
*
*
* ARGUMENTS EN ENTREE :
*   pa_DebListe : pointeur sur la liste de sockets.
*
* ARGUMENTS EN SORTIE :
*   NEANT.
*
* CODE RETOUR :
*  NEANT.
* CONDITION D'UTILISATION
*
*
* FONCTION
*   eliberation memoire occuppee par la liste de socket passee en argument.
*
------------------------------------------------------*/

void ecom_detruire_liste_sock(XZEXT_MASK **pa_DebListe)
{

    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_detruire_liste_sock";

    XZEXT_MASK *pl_Elm; /* pointeur sur l'elem. courant de la liste de sockets */

    /*A fermeture de toutes les sockets */
    /* POUR toutes les sockets de la liste */
    for (pl_Elm = *pa_DebListe; pl_Elm != NULL; pl_Elm = pl_Elm->Suivant)
    {
        /* Fermeture de la socket courante : appel ex_fin_cnx*/
        ex_fin_cnx(pl_Elm->desc_sock, pa_DebListe);
    }
    /*A FIN POUR */
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Extraction d'une demande LCR d'un msg. socket ds. une structure cmde. LCR.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* int	ecom_extraire_cmd_LCR( XZEXT_Msg_Socket *pa_MsgSock, XZEXT_Cmde_LCR *pa_CmdeLCR,
*				 int va_DesSockClnt)
*
*
* ARGUMENTS EN ENTREE :
*    pa_MsgSock	: pointeur sur le message contenant la demande LCR d'un tache TEzzz.
*    va_DesSockClnt : desc. socket tache cliente TEzzz ayant fait la demande LCR.
*
* ARGUMENTS EN SORTIE :
*
*    pa_CmdLCR	: pointeur sur la zone de stokage de la demande LCR depuillee.
*
* CODE RETOUR :
*   XDC_OK si tout est OK.
*   XDC_NOK Sinon.
* CONDITION D'UTILISATION
*
*   pa_MsgSock pointe sur un message recu sur une socket donnees contenant une demande LCR.
*
* FONCTION
*   depouillement de la demande LCR et stokage ds. la structure cmde. LCR.
*
------------------------------------------------------*/
int ecom_extraire_cmd_LCR(XZEXT_Msg_Socket *pa_MsgSock, XZEXT_Cmde_LCR *pa_CmdeLCR,
                          int va_DesSockClnt)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_extraire_cmd_LCR";

    char *pl_DebTrameLCR; /* pointe sur le debut de la trame LCR */
    XDY_Texte vl_Texte;   /* juste pour rec. le separateur de trame */

    /*A SI depouillement message socket echoue */
    if (sscanf(&(pa_MsgSock->msg[strlen(XZEXC_DEMANDE_LCR)]), "%d %s %d %d %s",
               &(pa_CmdeLCR->type_cmd),
               pa_CmdeLCR->rgs,
               &(pa_CmdeLCR->no_cmd),
               &(pa_CmdeLCR->prior),
               vl_Texte) != 5 ||
        (pl_DebTrameLCR = strstr(pa_MsgSock->msg, XZEXC_SEP_TRAME)) == NULL)
    {
        /*A ALORS trace " Demande LCR incorrecte <message> */
        XZST_03EcritureTrace(XZSTC_WARNING, "edir_traitement_msg_sock: Demande LCR incorrecte :\n <%s>\n", pa_MsgSock->msg);
        /*A retour XDC_NOK */
        return (XDC_NOK);
    }

    /*B recuperation trame LCR */
    pl_DebTrameLCR += strlen(XZEXC_SEP_TRAME);
    pa_MsgSock->msg[pa_MsgSock->len - (strstr(pa_MsgSock->msg, XZEXC_FIN_MSG_SOCK) == NULL ? 0 : strlen(XZEXC_FIN_MSG_SOCK))] = '\0';
    strcpy(pa_CmdeLCR->cmd_LCR, pl_DebTrameLCR);

    XZST_03EcritureTrace(XZSTC_INFO, "ecom_extraire_cmd_LCR: %d trame LCR <%s>\n",
                         strlen(XZEXC_SEP_TRAME), pa_CmdeLCR->cmd_LCR);

    /*B attachement desc. socket client a sa demande */
    pa_CmdeLCR->sock_clnt = va_DesSockClnt;

    /*B le type du message TEDI genere par cette demande LCR est XZEXC_MSG_REQ par defaut */
    pa_CmdeLCR->type_msg = XZEXC_MSG_REQ;

    /*B et donc le champ ack de la demande est NULL */
    pa_CmdeLCR->ack[0] = XZEXC_NULC;

    /*B si la cmde va generer un echange en blocs, le 1er a recevoir a le no. 0 */
    pa_CmdeLCR->no_blc_suivant = '0';

    /*A retour XDC_OK */
    return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Met une trame contenant des cars. non imprimable en format lisible.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* char	*ecom_rendre_trame_lisible( char *pa_Trame, int va_LongTrame, char *pa_TrameLisible );
*
*
* ARGUMENTS EN ENTREE :
*   pa_Trame : pointeur sur la liste de sockets.
*   va_LongTrame : longueur de la trame.
*   pa_TrameLisible	: pointeur sur buffer stockage trame lisible.
*
* ARGUMENTS EN SORTIE :
*   pa_TrameLisible	: pointeur sur la trame en format lisible.
*
* CODE RETOUR :
*  pl_TrameLisible : pointeur sur la trame en format lisible.
* CONDITION D'UTILISATION
*  pa_Trame doit etre de longueur  va_LongTrame avec va_LongTrame >= 0.
*
* FONCTION
*  Remplit pl_TrameLisible avec pa_Trame en remplacant les cars. non imprimable par
*  leur code hexa.
*
------------------------------------------------------*/

char *ecom_rendre_trame_lisible(char *pa_Trame, int va_LongTrame, char *pa_TrameLisible)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_rendre_trame_lisible";

    char *pl_TrameLisible; /* pointeur sur la trame lisible */
    size_t i, j;           /* compteur(s) */

    /*A allocage memoire pour la trame en format lisible */
    /*
        pl_TrameLisible = malloc( 4*sizeof(char)*va_LongTrame + 1 );
    */

    /*A mise en format lisible */
    pa_TrameLisible[0] = '\0';
    /*A POUR tous les cars. de la trame */
    for (i = 0; i < va_LongTrame; i++)
    {
        /*A SI car. courant imprimable */
        if (isprint(pa_Trame[i]))
        {
            /*A compter tous les cars. imprimables de suite */
            for (j = i + 1; j < va_LongTrame && isprint(pa_Trame[j]); j++)
                ;
            /*A recopier les ds. trame lisible */
            strncat(pa_TrameLisible, &pa_Trame[i], j - i);
            i = j - 1;
        }
        /*A SINON */
        else
            /*A recopie code hexa en format "[xx]" */
            sprintf(&pa_TrameLisible[strlen(pa_TrameLisible)], "[%.2X]", pa_Trame[i]);
        /*A FIN SI */
    }
    /*A FIN POUR */

    strcat(pa_TrameLisible, "");

    /* RETOUR trame lisible */
    return (pa_TrameLisible);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Initialisation  comm IP
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* int ecom_init_IP( 	XZEXT_IP_Config *pa_Config,
*			XZEXT_MASK **pa_SockList,
*			XZEXT_ST_Sockets *pa_desSocks
            )
*
*
* ARGUMENTS EN ENTREE :
*
* pa_Config	: pointeur sur structure stokage config non initialisee.
* pa_SockList	: pointeur sur la liste des sockets.
* pa_desSocks	: pointeur sur les sockets specifiques taches TELxi.
*
* ARGUMENTS EN SORTIE :
* pa_Config	: pointeur sur structure config mise a jour
* pa_SockList	: pointeur sur la liste des sockets mise a jour
* pa_desSocks	: pointeur sur les sockets specifiques taches TELximises a jour.
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*   lecture fichier config. lignes pour recuperer ds. pa_ConfigLigne la config de la ligne geree
*   connexion en mode client sur la socket serveur des terminaux.
*
------------------------------------------------------*/
int ecom_init_IP(XZEXT_IP_Config *pa_Config, XZEXT_MASK **pa_SockList, XZEXT_ST_Sockets *pa_DesSocks)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_init";

    XZEXT_MASK *vl_MasqueST = NULL; /* masque socket config. ligne geree */
    int vl_DesSock;                 /* socket comms. avec le ST par la console */
    char vl_CmdeST[512];            /* commande pour le ST */
    static int vl_CptEssai = 0;
    int vl_CodeRet = 0;
    XZEXT_MASK *pl_ListSock = NULL;

    XZST_03EcritureTrace(XZSTC_WARNING, "IN  : ecom_init_IP %s '%s'", pa_Config->adresseIP,
                         pa_Config->Nom_machine);

    /*A
     * lecture config. ligne serie geree.
     * remplissage donnees config. ds. la structure pa_ConfigLigne.
     */

    /*A SI lecture config. echoue, retour XDC_NOK */
    /*  if( ecom_lect_config_ligne_serie( pa_Config->Nom_machine,
                            &(pa_Config->ligne),
                            pa_Config->no_ligne)  != XDC_OK )
      {
          return( XDC_NOK );
      }*/
    pa_Config->ligne.NbEssais = 1;
    pa_Config->ligne.NbCarPref = 0;
    pa_Config->ligne.NbCarSuff = 0;
    /*A SI desc. socket lserie diff. de XZEXC_SOCKET_NULL ALORS fermeture socket */
    if (pa_DesSocks->lser != XZEXC_SOCKET_NULL)
    {
        ex_fin_cnx(pa_DesSocks->lser, pa_SockList);
        pa_DesSocks->lser = XZEXC_SOCKET_NULL;
    }

    /*A
     * connexion soket en mode client pour les comms.
     */

    XZST_03EcritureTrace(XZSTC_FONCTION, "ecom_init : cnx. client %s, %d ",
                         pa_Config->adresseIP, pa_Config->No_port);

    sleep(1);

    /*A SI connexion echoue */
    if (ex_cnx_client_ST(pa_Config->adresseIP,
                         pa_Config->No_port,
                         pa_SockList,
                         &vl_DesSock) != XDC_OK)
    {
        /*A ALORS */
        /*B ecriture trace : " Connexion socket ligne serie mode client impossible !" */
        XZST_03EcritureTrace(XZSTC_WARNING,
                             "ecom_init TEMIP : Connexion sur socket adresseIP <%s port %d> en mode client impossible !",
                             pa_Config->adresseIP, pa_Config->No_port);
        /*B mise XZEXC_SOCKET_NULL ds. pa_DesSocks->lser */
        pa_DesSocks->lser = XZEXC_SOCKET_NULL;

        /*A fermetur conexion avec la console ST */
        /*	   XZST_03EcritureTrace( XZSTC_WARNING, "fin_cnx %d",vl_DesSock);
                   ex_fin_cnx( vl_DesSock, &vl_MasqueST );*/
        /*B retour ELxiC_ST_NOK */
        return (XDC_NOK);
    }
    else
    {
        XZST_03EcritureTrace(XZSTC_WARNING, "socket TEMIP %d",
                             vl_DesSock);
    }

    /* memo du protocole de la socket*/
    for (pl_ListSock = *pa_SockList; pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant)
    {
        XZST_03EcritureTrace(XZSTC_WARNING, "socket TEMIP %s (%d)", pl_ListSock->adresse, pl_ListSock->desc_sock);

        if (pl_ListSock->desc_sock == vl_DesSock)
            break;
    }
    if (pl_ListSock != NULL)
        pl_ListSock->Protocole = pa_Config->protocole;
    /*A retour XDC_OK */
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : ecom_init ");
    return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Extraction d'une demande DIASER d'un msg. socket ds. une structure cmde. DIASER
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* int	ecom_extraire_cmd_DIASER( XZEXT_Msg_Socket *pa_MsgSock, XZEXT_Cmde_DIASER *pa_Cmde,
*				 int va_DesSockClnt)
*
*
* ARGUMENTS EN ENTREE :
*    pa_MsgSock	: pointeur sur le message contenant la demande LCR d'un tache TEzzz.
*    va_DesSockClnt : desc. socket tache cliente TEzzz ayant fait la demande LCR.
*
* ARGUMENTS EN SORTIE :
*
*    pa_CmdLCR	: pointeur sur la zone de stokage de la demande LCR depuillee.
*
* CODE RETOUR :
*   XDC_OK si tout est OK.
*   XDC_NOK Sinon.
* CONDITION D'UTILISATION
*
*   pa_MsgSock pointe sur un message recu sur une socket donnees contenant une demande LCR.
*
* FONCTION
*   depouillement de la demande LCR et stokage ds. la structure cmde. LCR.
*
------------------------------------------------------*/
int ecom_extraire_cmd_DIASER(XZEXT_Msg_Socket *pa_MsgSock, XZEXT_Cmde_DIASER *pa_Cmde,
                             int va_DesSockClnt)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_extraire_cmd_LCR";

    char *pl_DebTrame;  /* pointe sur le debut de la trame LCR */
    XDY_Texte vl_Texte; /* juste pour rec. le separateur de trame */

    /*A SI depouillement message socket echoue */
    if (sscanf(&(pa_MsgSock->msg[strlen(XZEXC_DEMANDE_DIASER)]), "%d %s %d %d %d %s",
               &(pa_Cmde->type_cmd),
               pa_Cmde->adresseIP,
               &(pa_Cmde->port),
               &(pa_Cmde->no_cmd),
               &(pa_Cmde->prior),
               vl_Texte) != 6 ||
        (pl_DebTrame = strstr(pa_MsgSock->msg, XZEXC_SEP_TRAME)) == NULL)
    {
        /*A ALORS trace " Demande LCR incorrecte <message> */
        XZST_03EcritureTrace(XZSTC_WARNING, "edir_traitement_msg_sock: Demande DIASER incorrecte :\n <%s>\n", pa_MsgSock->msg);
        /*A retour XDC_NOK */
        return (XDC_NOK);
    }

    /*B recuperation trame DIASER */
    pl_DebTrame += strlen(XZEXC_SEP_TRAME);
    pa_MsgSock->msg[pa_MsgSock->len - (strstr(pa_MsgSock->msg, XZEXC_FIN_MSG_SOCK) == NULL ? 0 : strlen(XZEXC_FIN_MSG_SOCK))] = '\0';
    strcpy(pa_Cmde->cmd, pl_DebTrame);

    XZST_03EcritureTrace(XZSTC_INFO, "ecom_extraire_cmd_DIASER: %d trame DIASER <%s>\n",
                         strlen(XZEXC_SEP_TRAME), pa_Cmde->cmd);

    /*B attachement desc. socket client a sa demande */
    pa_Cmde->sock_clnt = va_DesSockClnt;

    /*A retour XDC_OK */
    return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Extraction d'une demande LCR IP d'un msg. socket ds. une structure cmde. LCR.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* int	ecom_extraire_cmd_LCR_IP( XZEXT_Msg_Socket *pa_MsgSock, XZEXT_Cmde_LCR_IP *pa_CmdeLCR,
*				 int va_DesSockClnt)
*
*
* ARGUMENTS EN ENTREE :
*    pa_MsgSock	: pointeur sur le message contenant la demande LCR d'un tache TEzzz.
*    va_DesSockClnt : desc. socket tache cliente TEzzz ayant fait la demande LCR.
*
* ARGUMENTS EN SORTIE :
*
*    pa_CmdLCR	: pointeur sur la zone de stokage de la demande LCR depuillee.
*
* CODE RETOUR :
*   XDC_OK si tout est OK.
*   XDC_NOK Sinon.
* CONDITION D'UTILISATION
*
*   pa_MsgSock pointe sur un message recu sur une socket donnees contenant une demande LCR.
*
* FONCTION
*   depouillement de la demande LCR et stokage ds. la structure cmde. LCR.
*
------------------------------------------------------*/
int ecom_extraire_cmd_LCR_IP(XZEXT_Msg_Socket *pa_MsgSock, XZEXT_Cmde_LCR_IP *pa_CmdeLCR,
                             int va_DesSockClnt)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_extraire_cmd_LCR";

    char *pl_DebTrameLCR; /* pointe sur le debut de la trame LCR */
    XDY_Texte vl_Texte;   /* juste pour rec. le separateur de trame */

    /*A SI depouillement message socket echoue */
    if (sscanf(&(pa_MsgSock->msg[strlen(XZEXC_DEMANDE_LCR)]), "%d %s %d %s %d %d %s",
               &(pa_CmdeLCR->type_cmd),
               pa_CmdeLCR->adresseIP,
               &(pa_CmdeLCR->port),
               pa_CmdeLCR->rgs,
               &(pa_CmdeLCR->no_cmd),
               &(pa_CmdeLCR->prior),
               vl_Texte) != 7 ||
        (pl_DebTrameLCR = strstr(pa_MsgSock->msg, XZEXC_SEP_TRAME)) == NULL)
    {
        /*A ALORS trace " Demande LCR incorrecte <message> */
        XZST_03EcritureTrace(XZSTC_WARNING, "edir_traitement_msg_sock: Demande LCR incorrecte :\n <%s>\n", pa_MsgSock->msg);
        /*A retour XDC_NOK */
        return (XDC_NOK);
    }

    /*B recuperation trame LCR */
    pl_DebTrameLCR += strlen(XZEXC_SEP_TRAME);
    pa_MsgSock->msg[pa_MsgSock->len - (strstr(pa_MsgSock->msg, XZEXC_FIN_MSG_SOCK) == NULL ? 0 : strlen(XZEXC_FIN_MSG_SOCK))] = '\0';
    strcpy(pa_CmdeLCR->cmd_LCR, pl_DebTrameLCR);
    /*suppression espace inutile a la fin*/
    if (pa_CmdeLCR->cmd_LCR[strlen(pa_CmdeLCR->cmd_LCR) - 1] == ' ')
        pa_CmdeLCR->cmd_LCR[strlen(pa_CmdeLCR->cmd_LCR) - 1] = '\0';
    XZST_03EcritureTrace(XZSTC_INFO, "ecom_extraire_cmd_LCR_IP: trame LCR <%s>\n",
                         pa_CmdeLCR->cmd_LCR);

    /*B attachement desc. socket client a sa demande */
    pa_CmdeLCR->sock_clnt = va_DesSockClnt;

    /*B le type du message TEDI genere par cette demande LCR est XZEXC_MSG_REQ par defaut */
    pa_CmdeLCR->type_msg = XZEXC_MSG_REQ;

    /*B et donc le champ ack de la demande est NULL */
    pa_CmdeLCR->ack[0] = XZEXC_NULC;

    /*B si la cmde va generer un echange en blocs, le 1er a recevoir a le no. 0 */
    pa_CmdeLCR->no_blc_suivant = '0';

    /*A retour XDC_OK */
    return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Extraction d'une demande OPC d'un msg. socket ds. une structure cmde. OPC
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* int	ecom_extraire_cmd_OPC( XZEXT_Msg_Socket *pa_MsgSock, XZEXT_Cmde_OPC *pa_Cmde,
*				 int va_DesSockClnt)
*
*
* ARGUMENTS EN ENTREE :
*    pa_MsgSock	: pointeur sur le message contenant la demande OPC d'un tache TEzzz.
*    va_DesSockClnt : desc. socket tache cliente TEzzz ayant fait la demande OPC.
*
* ARGUMENTS EN SORTIE :
*
*    pa_CmdLCR	: pointeur sur la zone de stokage de la demande OPC depuillee.
*
* CODE RETOUR :
*   XDC_OK si tout est OK.
*   XDC_NOK Sinon.
* CONDITION D'UTILISATION
*
*   pa_MsgSock pointe sur un message recu sur une socket donnees contenant une demande OPC.
*
* FONCTION
*   depouillement de la demande OPC et stokage ds. la structure cmde. OPC.
*
------------------------------------------------------*/
int ecom_extraire_cmd_OPC(XZEXT_Msg_Socket *pa_MsgSock, XZEXT_Cmde_OPC *pa_Cmde,
                          int va_DesSockClnt)
{
    static char *version = "$Id: ex_mcom.c,v 1.20 2021/05/04 13:25:03 pc2dpdy Exp $ : ecom_extraire_cmd_LCR";

    char *pl_DebTrame;  /* pointe sur le debut de la trame OPC */
    XDY_Texte vl_Texte; /* juste pour rec. le separateur de trame */

    /*A SI depouillement message socket echoue */
    if (sscanf(&(pa_MsgSock->msg[strlen(XZEXC_DEMANDE_OPC)]), "%d %s %d %d %s",
               &(pa_Cmde->type_msg),
               (pa_Cmde->idBICE),
               &(pa_Cmde->no_cmd),
               &(pa_Cmde->prior),
               vl_Texte) != 5 ||
        (pl_DebTrame = strstr(pa_MsgSock->msg, XZEXC_SEP_TRAME)) == NULL)
    {
        /*A ALORS trace " Demande LCR incorrecte <message> */
        XZST_03EcritureTrace(XZSTC_WARNING, "ecom_extraire_cmd_OPC: typemsg:%d idBICE:%s Nocmd:%d Prior:%d texte:%s", (pa_Cmde->type_msg),
                             (pa_Cmde->idBICE), (pa_Cmde->no_cmd), (pa_Cmde->prior),
                             vl_Texte);
        XZST_03EcritureTrace(XZSTC_WARNING, "edir_traitement_msg_sock: Demande OPC incorrecte :\n <%s>\n", pa_MsgSock->msg);
        /*A retour XDC_NOK */
        return (XDC_NOK);
    }

    /*B recuperation trame OPC */
    pl_DebTrame += strlen(XZEXC_SEP_TRAME);
    pa_MsgSock->msg[pa_MsgSock->len - (strstr(pa_MsgSock->msg, XZEXC_FIN_MSG_SOCK) == NULL ? 0 : strlen(XZEXC_FIN_MSG_SOCK))] = '\0';
    strcpy(pa_Cmde->cmd, pl_DebTrame);

    XZST_03EcritureTrace(XZSTC_INFO, "ecom_extraire_cmd_OPC: len separateur:%d trame OPC <%s>\n",
                         strlen(XZEXC_SEP_TRAME), pa_Cmde->cmd);

    /*B attachement desc. socket client a sa demande */
    pa_Cmde->sock_clnt = va_DesSockClnt;

    /*A retour XDC_OK */
    return (XDC_OK);
}
