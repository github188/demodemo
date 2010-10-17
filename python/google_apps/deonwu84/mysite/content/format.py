import re

class Transform(object):
  """Abstraction for a regular expression transform.

  Transform subclasses have two properties:
     regexp: the regular expression defining what will be replaced
     replace(MatchObject): returns a string replacement for a regexp match

  We iterate over all matches for that regular expression, calling replace()
  on the match to determine what text should replace the matched text.

  The Transform class is more expressive than regular expression replacement
  because the replace() method can execute arbitrary code to, e.g., look
  up a WikiWord to see if the page exists before determining if the WikiWord
  should be a link.
  """
  def run(self, content):
    """Runs this transform over the given content.

    We return a new string that is the result of this transform.
    """
    parts = []
    offset = 0
    for match in self.regexp.finditer(content):
      parts.append(content[offset:match.start(0)])
      parts.append(self.replace(match))
      offset = match.end(0)
    parts.append(content[offset:])
    return ''.join(parts)


class AutoLink(Transform):
  """A transform that auto-links URLs."""
  def __init__(self):
    self.regexp = re.compile(r'([^"])\b((http|https)://[^ \t\n\r<>\(\)&"]+' \
                             r'[^ \t\n\r<>\(\)&"\.])')

  def replace(self, match):
    url = match.group(2)
    return match.group(1) + '<a class="autourl" href="%s">%s</a>' % (url, url)

class SimpleHtmlFormat(Transform):
  """A transform that auto-links URLs."""
  def __init__(self):
    self.regexp = re.compile(r'(\n)')
    #/^<([a-z]+)([^<]+)*(?:>(.*)<\/\1>|\s+\/>)$/  

  def replace(self, match):
    return "<br/>"

class BlockHtmlFormat(Transform):
  """A transform that auto-links URLs."""
  def __init__(self):
    self.regexp = re.compile(r'(={1,3}|\*|~)(.*?)\1')
    #/^$/  

  def replace(self, match):
    mark = match.group(1)
    tag = {"=":"h1",
           "==":"h2",
           "===":"h3",
           "*":"b",
           "~":"i",
           }.get(mark, "div")
    
    context = match.group(2)
    
    return "<%s>%s</%s>" % (tag, context, tag)
