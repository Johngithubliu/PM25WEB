<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- Edited with XML Spy v2007 (http://www.altova.com) -->
<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/">
  <html>
  <head><meta http-equiv="refresh" content="30"></meta></head>
  <body>
    <h2 align="center" style="font-size:400%">Air quality of <xsl:value-of select="AIRQ/POSITION"/></h2>

	<xsl:for-each select="AIRQ/Item[Desc='PM2.5(ug/m3)@CF=1']">
      
	
	<xsl:choose>
          <xsl:when test="Value &gt; 500">
    	<div style="background-color:#7e0023;text-align:center;font-size:350%">
            Hazardous</div>	
          </xsl:when>
          <xsl:when test="Value &gt; 250">
	<div style="background-color:#660099;text-align:center;font-size:350%">
             Very unhealthly</div>
          </xsl:when>
	<xsl:when test="Value &gt; 150">
          <div style="background-color:#cc0033;text-align:center;font-size:350%">
             Unhealthly</div>
          </xsl:when>
	<xsl:when test="Value &gt; 65">
          <div style="background-color:#ff9933;text-align:center;font-size:350%">
             Unhealthly for sensitive Groups</div>
          </xsl:when>
	<xsl:when test="Value &gt; 40">
          <div style="background-color:#ffde33;text-align:center;font-size:350%">
             Moderate</div>
          </xsl:when>
	<xsl:when test="Value &gt; 15">
         <div style="background-color:#009966;text-align:center;font-size:350%">
             Good</div>          </xsl:when>

        <xsl:otherwise>
         <div style="background-color:#009966;text-align:center;font-size:350%">
             Good</div>
          </xsl:otherwise>
        </xsl:choose>

  

      </xsl:for-each>


<div align="center" style="font-size:200%">
Update time:<xsl:value-of select="/AIRQ/UPDATA"/>
</div>

  <table width="50%" border="1" align="center" style="font-size:350%">
  
    <tr bgcolor="#9acd32">
        <th>Item</th>
        <th>Value</th>
      </tr>


	<xsl:for-each select="AIRQ/Item">
      <tr>
        <td><xsl:value-of select="Desc"/></td>
      	<td><xsl:value-of select="Value"/></td>
	
      </tr>
	</xsl:for-each >
    </table>
    
  </body>
  </html>
</xsl:template>
</xsl:stylesheet>