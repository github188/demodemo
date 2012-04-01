
CRAWLER = {'download': ('sailing.worker.crawler.Site.DownLoadSite', 
                          {'patterns': ('/guess/(last|hot)(.*?)', )}),
            'guess_topic': ('guess_crwaler.GuessTopicCrwaler', {})
		  }


CROWERS_MAPPING = (('.*\.html', ('guess_topic', 'download', )),	
                    #('/guess/\d+/', ('guess_topic', )
					)
					
START_INDEX = "http://www.uqude.com/guess/hot/"					