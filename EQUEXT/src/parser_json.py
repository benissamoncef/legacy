import json,sys;
objreponse=json.load(sys.stdin);
result= open('/tmp/polyvox_response.txt', 'w')
if "KO" in objreponse["status"]:
    result.write("KO "+objreponse["result"])
else:
    i=0;
    while i < len(objreponse["result"]):
        key=objreponse["result"][i];
        if len(key["subType"][0])> 1:
            subtype=key["subType"][0]
        else:
            subtype=key["subType"]
        result.write("type "+key["type"]+"\n")
        result.write("id "+key["id"]+"\n")
        result.write("version "+key["version"]+"\n")
        result.write("subType "+subtype+"\n")
        result.write("probability "+key["probability"]+"\n")
        result.write("creationTime "+key["creationTime"]+"\n")
        result.write("lastModificationTime "+key["lastModificationTime"]+"\n")
        result.write("roadNumber "+key["roadNumber"]+"\n")
        result.write("direction "+key["direction"]+"\n")
        result.write("pr "+key["pr"]+"\n")
        result.write("distance "+str(key["distance"])+"\n")
        result.write("end "+str(key["end"])+"\n")
#        result.write("supplier "+key["supplier"]+"\n")
	i=i+1;
result.close()
