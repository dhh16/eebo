function wrap(text, width) {
    text.each(function() {
	var text = d3.select(this),
	    words = text.text().split(/\s+/).reverse(),
	    word,
	    line = [],
	    lineNumber = 0,
	    lineHeight = 1.1, // ems
	    y = text.attr("y"),
	    dy = parseFloat(text.attr("dy")),
	    tspan = text.text(null).append("tspan").attr("x", 0).attr("y", y).attr("dy", dy + "em");
	while (word = words.pop()) {
	    line.push(word);
	    tspan.text(line.join(" "));
	    console.log(tspan.node().getComputedTextLength());
	    if (tspan.node().getComputedTextLength() > width) {
		line.pop();
		tspan.text(line.join(" "));
		line = [word];
		tspan = text.append("tspan").attr("x", 0).attr("y", y).attr("dy", ++lineNumber * lineHeight + dy + "em").text(word);
	    }
	}
    });
}
