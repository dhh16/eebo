host=shell.cs.helsinki.fi
host_dir=/cs/home/hxiao/public_html/eebo

tnc=text_network_of_copying_relation.graphml
anc=author_network_of_copying_relation.graphml
ssh=ssh ${host}
cv=civil-war
cvt=civil-war-text
sts=sixteen-author-short
stl=sixteen-author-long
stt=sixteen-text

all: d3_${stl}
	echo "done"

text_network:
	scp output-1599/${tnc} ${host}:${host_dir}
	ssh ${host} chmod a+r ${host_dir}/${tnc}

author_network:
	scp output/${anc} ${host}:${host_dir}
	ssh ${host} chmod a+r ${host_dir}/${anc}

d3_${cv}:
	ssh ${host} rm -rf ${host_dir}/${cv}
	scp -r html/ ${host}:${host_dir}/${cv}
	scp data/${cv}/graph.json ${host}:${host_dir}/${cv}
	${ssh} chmod -R a+rx ${host_dir}/${cv}

d3_${cvt}:
	ssh ${host} rm -rf ${host_dir}/${cvt}
	scp -r html/ ${host}:${host_dir}/${cvt}
	scp data/${cvt}/graph.json ${host}:${host_dir}/${cvt}
	${ssh} chmod -R a+rx ${host_dir}/${cvt}

d3_${sts}:
	ssh ${host} rm -rf ${host_dir}/${sts}
	scp -r html/ ${host}:${host_dir}/${sts}
	scp data/${sts}/graph.json ${host}:${host_dir}/${sts}
	${ssh} chmod -R a+rx ${host_dir}/${sts}

d3_${stl}:
	ssh ${host} rm -rf ${host_dir}/${stl}
	scp -r html/ ${host}:${host_dir}/${stl}
	scp data/${stl}/graph.json ${host}:${host_dir}/${stl}
	${ssh} chmod -R a+rx ${host_dir}/${stl}

d3_${stt}:
	ssh ${host} rm -rf ${host_dir}/${stt}
	scp -r html/ ${host}:${host_dir}/${stt}
	scp data/${stt}/graph.json ${host}:${host_dir}/${stt}
	${ssh} chmod -R a+rx ${host_dir}/${stt}
