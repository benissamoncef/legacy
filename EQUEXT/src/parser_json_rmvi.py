import json,sys;
objreponse=json.load(sys.stdin);
result= open('/produits/migrazur/appliSD/tmp/rmvi_response.txt', 'w')
if "erreur" in objreponse["status"]:
    result.write("erreur "+objreponse["result"])
else:
    i=0;
    while i < len(objreponse["list"]):
        key=objreponse["list"][i];
        result.write("position "+key["position"]+"\n")
        result.write("tempsParcours "+str(key["tempsParcours"])+"\n")
        result.write("dlp "+str(key["dlp"])+"\n")
        result.write("typeIntervenant "+key["typeIntervenant"]+"\n")
        result.write("vitesse "+str(key["vitesse"])+"\n")
        result.write("idintervenant "+str(key["idintervenant"])+"\n")
        result.write("tauxFiabilite "+str(key["tauxFiabilite"])+"\n")
        result.write("note "+str(key["note"])+"\n")
	i=i+1;
result.close()
