<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:template match="bundled">
        <!-- get dependencies -->
uds.bundled.projects=\
<xsl:for-each select="dependency"><xsl:if test="@kind='project'">..<xsl:value-of
select="@reference"/>,\</xsl:if></xsl:for-each>
</xsl:template>
</xsl:stylesheet>