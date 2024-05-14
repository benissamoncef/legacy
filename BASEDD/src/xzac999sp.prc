/*E*/
/*  Fichier : $Id: xzac999sp.prc,v 1.4 2021/04/06 09:55:01 gesconf Exp $     Release : $Revision: 1.4 $        Date : $Date: 2021/04/06 09:55:01 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzac999sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* PNI	15/12/14	: Creation
* LCL	05/01/21	: Modification pour PRA MOVIS DEM-SAE93 1.1
* PNI	30/03/21	: Ajout appels XZAR;03 (pour keepalive des TASRV_xx) et SQLHIS.PRC..XZAC999 (keepalive SQL_HIS) 1.2 SAE-238
* PNI	06/02/21	: Ajout appels TASRV_DP...ARRP03 (pour keepalive des TASRV_xx)  SAE-238
* PNI	21/09/21	: Ajout appels TASRV_xx...ARRP03 pour DP et VC (pour keepalive des TASRV_xx)  SAE-238
* PNI	14/12/21	: Ajout appels SQL_HIS.PRC..XZAC99 pour VC et DP
* JPC	27/07/23	: Suppression du metier et de tous les appels. On produit une coquille vide SAE-505
------------------------------------------------------ */

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne  XDC_OK
* 
* Sequence d'appel
* SP	XZAC999
* 
* Arguments en entree
* 
* Arguments en sortie

* 
* Code retour
* XDC_OK
* XDC_NOK	: site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* Rien
* 
--------------------------------------------------------*/

use PRC
go
drop proc XZAC999
go

create procedure XZAC999
	@va_site_in tinyint = null								
as
declare @vl_Status int,
	@cle tinyint,
	@vl_serveur_pra char(6)

	select @cle=0

	return XDC_OK
go