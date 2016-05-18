host=shell.cs.helsinki.fi
host_dir=/cs/home/hxiao/public_html/eebo

tnc=text_network_of_copying_relation.graphml
anc=author_network_of_copying_relation.graphml
ssh=ssh ${host}
cv=civil-war
all: d3_civil_war
	echo "done"

text_network:
	scp output/${tnc} ${host}:${host_dir}
	ssh ${host} chmod a+r ${host_dir}/${tnc}

author_network:
	scp output/${anc} ${host}:${host_dir}
	ssh ${host} chmod a+r ${host_dir}/${anc}

d3_civil_war:
	ssh ${host} rm -rf ${host_dir}/${cv}
	scp -r html/ ${host}:${host_dir}/${cv}
	scp data/${cv}/graph.json ${host}:${host_dir}/${cv}
	${ssh} chmod -R a+rx ${host_dir}/${cv}
