/*E*/
/*Fichier :  $Id: xzao991.c,v 1.3 2018/06/06 16:07:58 devgfi Exp $      Release : $Revision: 1.3 $        Date : $Date: 2018/06/06 16:07:58 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao991.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       21 Nov 2007    : Creation DEM 662
* JPL		13/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.2
* JPL		07/02/18 : Migration Linux 64 bits (DEM 1274) : Initialisation structure de donnees equipement  1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao991.c,v 1.3 2018/06/06 16:07:58 devgfi Exp $ : xzao991" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des Equipements et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao991_Lire_Config_Equipement ( FILE               *pa_Fd,
                                     XDY_FonctionInt    pa_FonctionUtil,
                                     XDY_Entier         *pa_Resultat ) 
                        
/*
* ARGUMENTS EN ENTREE :
*
* FILE            pa_Fd
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
*       XZAOT_ConfEqt     contenu de ligne
*       XDY_Entier         NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
    static char *version = "$Id: xzao991.c,v 1.3 2018/06/06 16:07:58 devgfi Exp $ : xzao991_Lire_Config_Equipement" ;
   
    XZAOT_ConfEqt    vl_EqtGTC;
    char             vl_Lig[255];
    char             *vl_Texte;
    char             vl_Texte_out[255];
    XDY_Entier             vl_num_ligne = 0;
    int              vl_retcode = 0;
    XDY_Entier       vl_Resultat_out = 0;

    /*A
    ** Test sur le file descriptor passe en parametre d'entree 
    */

    if (pa_Fd == NULL)
    {
        XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao991: fichier non existant");
        return ( XDC_NOK );
    }
 
    /*A
    ** Boucle effectuant la lecture ligne par ligne du fichier correspondant È pa_Fd
    */
    
    while ( ( vl_retcode = XZAO000_LireLigneFich ( pa_Fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
    {
        /*A
        ** Comptage des lignes lues du fichier pa_Fd 
        */
    
        vl_num_ligne++; 
       
        if (vl_Resultat_out != XZAO_EOF )
        { 
			/* RAZ structure */
			memset(&vl_EqtGTC, 0, sizeof(XZAOT_ConfEqt));
			
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
                ** Extraction des differents champs de vl_District
                */

                vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             
				XZST_03EcritureTrace(XZSTC_INFO, "xzao991 : Analyse de %s", vl_Texte);
                if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 100 /*JBL XDC_Lon_NomEqt*/, vl_Texte_out ) ) != XDC_OK )
                {  
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : Saisie  non valide");
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : retourne %d",XDC_NOK);
                    return ( XDC_NOK );
                }
               sprintf( vl_EqtGTC.TypeEqt, "%s", vl_Texte_out );


                vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
				XZST_03EcritureTrace(XZSTC_INFO, "xzao991 : Analyse de %s", vl_Texte);
                if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
                {  
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : Saisie  non valide");
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : retourne %d",XDC_NOK);
                    return ( XDC_NOK );
                }
             
                sprintf ( vl_EqtGTC.NomEqt, "%s", vl_Texte_out );

                vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
				XZST_03EcritureTrace(XZSTC_INFO, "xzao991 : Analyse de %s", vl_Texte);
                if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomAuto, vl_Texte_out ) ) != XDC_OK )
                {  
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : Saisie  non valide");
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : retourne %d",XDC_NOK);
                    return ( XDC_NOK );
                }

                sprintf ( vl_EqtGTC.Autoroute, "%s", vl_Texte_out );


                vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
				XZST_03EcritureTrace(XZSTC_INFO, "xzao991 : Analyse de %s", vl_Texte);
                if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
                {  
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : Saisie  non valide");
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : retourne %d",XDC_NOK);
                    return ( XDC_NOK );
                }
             
                sprintf ( vl_EqtGTC.NomMaitre, "%s", vl_Texte_out );


                vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
				XZST_03EcritureTrace(XZSTC_INFO, "xzao991 : Analyse de %s", vl_Texte);
                if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
                {  
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : Saisie  non valide");
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : retourne %d",XDC_NOK);
                    return ( XDC_NOK );
                }

                vl_EqtGTC.PR = (XDY_PR) atol ( vl_Texte_out );

                vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

				XZST_03EcritureTrace(XZSTC_INFO, "xzao991 : Analyse de %s", vl_Texte);
                if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
                {  
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : Saisie  non valide");
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : retourne %d",XDC_NOK);
                    return ( XDC_NOK );
                }

                vl_EqtGTC.Sens = (XDY_Sens) atoi ( vl_Texte_out );


                vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
				XZST_03EcritureTrace(XZSTC_INFO, "xzao991 : Analyse de %s", vl_Texte);
                if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
                {  
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : Saisie  non valide");
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : retourne %d",XDC_NOK);
                    return ( XDC_NOK );
                }

                vl_EqtGTC.Voie = (XDY_Voie) atoi ( vl_Texte_out );


                vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
				XZST_03EcritureTrace(XZSTC_INFO, "xzao991 : Analyse de %s", vl_Texte);
                if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
                {  
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : Saisie  non valide");
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : retourne %d",XDC_NOK);
                    return ( XDC_NOK );
                }

                vl_EqtGTC.Inverse = (XDY_Octet) atoi ( vl_Texte_out );


                vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
				XZST_03EcritureTrace(XZSTC_INFO, "xzao991 : Analyse de %s", vl_Texte);
                if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
                {  
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : Saisie  non valide");
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao991 : retourne %d",XDC_NOK);
                    return ( XDC_NOK );
                }
             
                sprintf ( vl_EqtGTC.Domaine, "%s", vl_Texte_out );

                /*A
                ** Verification que la fonction utilisateur passee en parametre est non NULL 
                */
    
                if (pa_FonctionUtil != NULL)
                {
                    /*B
                    ** Appel de la fonction utilisateur passee en parametre 
                    */
					
				    XZST_03EcritureTrace( XZSTC_INTERFACE,"xzao991 Appel de pa_FonctionUtil avec: Type = %s\tNom = %s\tAutoroute = %s\tNomMaitre = %s\tPR = %ld\tSens = %d\tVoie = %d\tInverse = %d\tDomaine = %s",
				            vl_EqtGTC.TypeEqt,
				            vl_EqtGTC.NomEqt,
				            vl_EqtGTC.Autoroute,
				            vl_EqtGTC.NomMaitre,
				            vl_EqtGTC.PR,
				            vl_EqtGTC.Sens,
				            vl_EqtGTC.Voie,
				            vl_EqtGTC.Inverse,
				            vl_EqtGTC.Domaine );
					
         
                    if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_EqtGTC, vl_num_ligne ) ) != XDC_OK )
                    {    
                        XZST_03EcritureTrace(XZSTC_WARNING,"appel de FonctionUtilisateur s'est mal deroule");
                        return ( XDC_NOK );
                    }  
                } 
                else
                {
                    /*B
                    ** Fonction utilisateur inexistante =>
                    ** Retour du status XDC_NOK et du numero de ligne 
                    */

                    XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao991: echec de l'appel de la fonction utilisateur");
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
            *pa_Resultat = 0;
            return ( XDC_OK );
        }
    }
 
    /*A
    ** Fin de la boucle de lecture du fichier pa_Fd 
    ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL 
    */
   
    *pa_Resultat = 0;

    return XDC_OK;

}   

/* Fin de la fonction xzao991_Lire_Config_Equipement */

