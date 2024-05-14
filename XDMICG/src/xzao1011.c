/*E*/
/*Fichier :  $Id: xzao1011.c,v 1.2 2016/07/13 12:43:05 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2016/07/13 12:43:05 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao1011.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ        15 Nov 2007    : Creation DEM 662
* JPL	13/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao1011.c,v 1.2 2016/07/13 12:43:05 devgfi Exp $ : xzao1011" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des Sequences des Eqts domaine EXP et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao1011_Lire_Config_Seq_EXP ( FILE            *pa_Fd,
                                     XDY_FonctionInt pa_FonctionUtil,
                                     XDY_Entier      *pa_Resultat ) 
                        
/*
* ARGUMENTS EN ENTREE :
*
* FILE               pa_Fd
* XDY_FonctionInt    pa_FonctionUtil
*
* ARGUMENTS EN SORTIE :    
*
* XDY_Entier        *pa_Resultat:OK=NULL ou ligne de l'erreur du systeme
*
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*    Appelle la fonction utilisateur avec les arguments:
*       XZAOT_ConfSeqEXP     contenu de ligne
*       XDY_Entier              NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
    static char *version = "$Id: xzao1011.c,v 1.2 2016/07/13 12:43:05 devgfi Exp $ : xzao1011_Lire_Config_Seq_EXP" ;
   
    XZAOT_ConfSeqEXP         vl_SeqEXP;
    char                     vl_Lig[255];
    char                     *vl_Texte;
    char                     vl_Texte_out[255];
    XDY_Entier                     vl_num_ligne    = 0;
    int                      vl_retcode      = 0;
    XDY_Entier               vl_Resultat_out = 0;

    /*A
    ** Test sur le file descriptor passe en parametre d'entree 
    */

    if (pa_Fd == NULL)
    {
        XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao1011: fichier non existant");
        return ( XDC_NOK );
    }

    /*A
    ** Boucle effectuant la lecture ligne par ligne du fichier correspondant a pa_Fd
    */
    
    while ( ( vl_retcode = XZAO000_LireLigneFich ( pa_Fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
    {
        /*A
        ** Comptage des lignes lues du fichier pa_Fd 
        */

        vl_num_ligne++; 

        if (vl_Resultat_out != XZAO_EOF )
        { 
            /*A
            ** Affectation du parametre de sortie egal au numero de la ligne lue
            */

            *pa_Resultat = vl_num_ligne;
             
            /*A
            ** Verification que la ligne n'est pas un commentaire
            ** Presence du caractere #
            */

            vl_retcode = strcspn ( vl_Lig, XZAO_COMMENT );

            if (vl_retcode != XDC_OK) 
            {
                /*A
                ** Extraction des differents champs
                */

                vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );

                if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
                {  
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao1011 : Saisie  non valide");
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao1011 : retourne %d",XDC_NOK);
                    return ( XDC_NOK );
                }
             
                vl_SeqEXP.Creation = atoi ( vl_Texte_out );

                vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

                if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
                {  
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao1011 : Saisie  non valide");
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao1011 : retourne %d",XDC_NOK);
                    return ( XDC_NOK );
                }

                strcpy ( vl_SeqEXP.NomSequence, vl_Texte_out );


                vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

                if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
                {  
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao1011 : Saisie  non valide");
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao1011 : retourne %d",XDC_NOK);
                    return ( XDC_NOK );
                }
             
                vl_SeqEXP.Type = atoi ( vl_Texte_out );

                /*A
                ** Verification que la fonction utilisateur passee en parametre est non NULL 
                */

                if (pa_FonctionUtil != NULL)
                {
                    /*B
                    ** Appel de la fonction utilisateur passee en parametre 
                    */

                    if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_SeqEXP, vl_num_ligne ) ) != XDC_OK )
                    {    
                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao1011 : appel de FonctionUtilisateur s'est mal deroule");
                        return ( XDC_NOK );
                    }  
                } 
                else
                {
                    /*B
                    ** Fonction utilisateur inexistante =>
                    ** Retour du status XDC_NOK et du numero de ligne 
                    */

                    XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao1011: echec de l'appel de la fonction utilisateur");
                    return ( XDC_NOK );
                }
            }
        }
       
        else
       
        /* A
        ** Fin de lecture du fichier pa_Fd 
        ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL 
        */
       
        {  
            *pa_Resultat = NULL;
            return ( XDC_OK );
        }
    }
 
    /*A
    ** Fin de la boucle de lecture du fichier pa_Fd 
    ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL 
    */

    *pa_Resultat = NULL;

    return XDC_OK;

}   

/* Fin de la fonction */

