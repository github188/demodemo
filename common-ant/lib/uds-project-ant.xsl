<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="jarname">
</xsl:template>
<xsl:template match="bundled">
<!-- create dependency copy script -->
<project name="dynamic-copy-script" default="copy-dependencies" basedir=".">

<target name="copy-dependencies">
<xsl:for-each select="dependency">
<xsl:if test="@kind='jar'">
    <xsl:if test="starts-with(@reference, '/')">
        <copy>
            <xsl:attribute name="file">..<xsl:value-of select="@reference"/></xsl:attribute>
            <xsl:attribute name="todir">bundled-lib</xsl:attribute>
        </copy>
    </xsl:if>
    <xsl:if test="not(starts-with(@reference, '/'))">
        <copy>
            <xsl:attribute name="file">./<xsl:value-of select="@reference"/></xsl:attribute>
            <xsl:attribute name="todir">bundled-lib</xsl:attribute>
        </copy>
    </xsl:if>
</xsl:if>    
<xsl:if test="@kind='ext'">
    <copy>
        <xsl:attribute name="file"><xsl:value-of select="@reference"/></xsl:attribute>
        <xsl:attribute name="todir">bundled-lib</xsl:attribute>
    </copy>
</xsl:if>
</xsl:for-each>
</target>

</project>
</xsl:template>
</xsl:stylesheet>