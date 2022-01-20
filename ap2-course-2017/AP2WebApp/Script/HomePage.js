
/**
 * initialize the header color
 */
function initColor() {

    var htag = document.getElementsByTagName("h1");
    htag[0].onclick = chColor;
}

/**
 *change the header color with every click
 */
function chColor() {
    var randi = '#' + Math.floor((Math.random()*16777215)).toString(16);
    this.style.color = randi;
    onload="alert('you entered the greatest game on earth, are you ready??')";
}

/**
 * play a song after clicking the surprise button
 */
function surprise() {
   
    var song = document.getElementById("eden");
    var isVis = song.style.visibility !== "visible";
    song.style.visibility = isVis ?"visible" : "hidden";
    song.play();
}

/**
 * load the upper nav bar as the page is lodaed
 */
$(document).ready(function()
{
    $("#site-header").load("MenuNav.html");
});
onload = initColor;
