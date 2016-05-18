var url='graph.json'
var W=500,
    H=500;
var force = d3.layout.force()
    .charge(-200)
    .linkDistance(10)
    .size([W, H]);

var svg = d3.select("body").append("svg")
    .attr("width", W)
    .attr("height", H);

// svg.append("svg:defs")
// 	.append("svg:marker")
// 	.attr("id", "triangle")
// 	.attr("viewBox", "0 -5 10 10")
// 	.attr("refX", 15)
// 	.attr("refY", -1.5)
// 	.attr("markerWidth", 5)
// 	.attr("markerHeight", 5)
// 	.attr("orient", "auto")
// 	.append("svg:path")
// 	.attr("d", "M0,-5L10,0L0,5");

d3.json(url, function(error, graph) {
    if (error) throw error;
    var tip = d3.tip()
	.attr('class', 'd3-tip')
	// .offset()
	.html(function(d){
	    return '';
	});
    
    svg.call(tip);
    
    console.log('#nodes:', graph.nodes.length);

    // var people_color = d3.scale.ordinal()
    // 	.domain(participants)
    // 	.range(d3.scale.category10().range())
    console.log(graph);
    force
	.nodes(graph.nodes)
	.links(graph.edges)
	.start();

    console.log('here');
    
    var glinks = svg.selectAll('g.glink')
	.data(graph.edges)
	.enter()
	.append('g')
	.classed('glink', true);
    
    var link = glinks.append("line")
	.attr("class", "link")
	.attr("marker-end", "url(#triangle)")
	.attr("stroke", 'black')
	.attr("stroke-width", 2)
	.attr("opacity", 0.8);
    
    var costs = _.map(graph.edges, function(e){
	return e['c'];
    });
    var link_weight = d3.scale.quantize()
	.domain([d3.min(costs), d3.max(costs)])
	.range([100, 200, 300, 400, 500, 600, 700, 800, 900]);

    function dragmove(d) {
	d3.select(this)
	    .attr('transform', function(d){
		return 'translate(' + [d3.event.x, d3.event.y] + ')';
	    });
    }

    var drag = d3.behavior.drag()
	.on("drag", dragmove);

    var link_labels = glinks.append("text")
	.text('')
	.attr('font-size', 10)
	.attr('font-weight', function(d){
	    return 2;
	})

    var nodes = svg.selectAll('circle.node')
	.data(graph.nodes)
	.enter()
	.append('circle')
	.attr("class", "node")
	.attr('stroke', 'black')
	.attr('stroke-width', 2)
	.attr("r", function(d){
	    return 10;
	})
	.style("fill", function(d){
	    return 'red';
	})
	.attr('opacity', 0.8)
	.call(force.drag)
	.on('click', function(d){
	    var active_r = 15;
	    var deactive_r = 5;
	    var text = node_labels.filter(function(other){
		return other['message_id'] == d['message_id'];
	    });
	    var me = d3.select(this)
	    if (text.style('display') == 'none'){
		text.style('display', 'inline');
		text.style('fill', me.style('fill'));
		// me.attr('opacity', 1.0);
		me.attr('r', active_r);
	    }
	    else{
		text.style('display', 'none');
		// d3.select(this).attr('opacity', 0.3);
		me.attr('r', deactive_r);
	    }
	})
	.on('mouseover', tip.show)
	.on('mouseout',  tip.hide);

    var node_labels = svg.selectAll('text.node_label')
	.data(graph.nodes)
	.enter()
	.append('text')
	.attr("class", "node_label")
	.text(config.node.label)
	.attr('font-size', 16)
	.attr('font-weight', 'bold')
	.style('display', 'none')
	.style('text-anchor', 'middle')
	.call(drag);


    force.on("tick", function() {
	nodes[0].x = config.svg.width / 2;
	nodes[0].y = config.svg.height / 2;

	link.attr("x1", function(d) { return d.source.x; })
	    .attr("y1", function(d) { return d.source.y; })
	    .attr("x2", function(d) { return d.target.x; })
	    .attr("y2", function(d) { return d.target.y; });

	// gnode.attr("cx", function(d) { return d.x; })
	// 	.attr("cy", function(d) { return d.y; });
	nodes.attr("transform", function(d) { 
	    return 'translate(' + [d.x, d.y] + ')'; 
	}); 

	link_labels.attr("transform", function(d) { 
	    var x = (d.source.x + d.target.x) / 2;
	    var y = (d.source.y + d.target.y) / 2;
	    return 'translate(' + [x, y] + ')';
	});
	node_labels.attr('transform', function(d){
	    return 'translate(' + [d.x, d.y] + ')'; 
	})

    });
});

