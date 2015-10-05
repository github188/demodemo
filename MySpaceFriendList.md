# OpenSocial vs MyOpenSpace #

OpenSocial 是一个开放的标准，由Google, Myspace, Hi5等维护。http://developer.myspace.com/community/myspace/opensocialref.aspx

MyOpenSpace 是OpenSocial标准的实现，并加入了一些Myspace特有的功能。MyOpenSpace.js


# Introduction #

```
function init() {
    MYOS_TRACE = true;  trace('Init...');
    var os = opensocial.Container.get();
    var dataReqObj = os.newDataRequest();
 
    var viewerReq = os.newFetchPersonRequest(opensocial.IdSpec.PersonId.VIEWER);
    dataReqObj.add(viewerReq, "viewer");
	
    //var req = opensocial.newDataRequest();
   
    params = {};
    params[opensocial.IdSpec.Field.USER_ID] = opensocial.IdSpec.PersonId.VIEWER;
    params[opensocial.IdSpec.Field.GROUP_ID] = opensocial.IdSpec.GroupId.FRIENDS;

    var opt_params = {};
    opt_params[opensocial.DataRequest.PeopleRequestFields.FIRST] = 1;
    opt_params[opensocial.DataRequest.PeopleRequestFields.MAX] = 40;

    var idspec = opensocial.newIdSpec(params);
    dataReqObj.add(dataReqObj.newFetchPeopleRequest(idspec, opt_params), 'viewerFriends');
	
    trace('Sending...');
    dataReqObj.send(dataLoadCallback);
}
 
function dataLoadCallback(dataResponse) {
    trace('Got data...');
	
    if (dataResponse.hadError()) {
        var data = dataResponse.get("friends");
        trace(data.getErrorCode() + '\n' + data.getErrorMessage());
    } else {
        var viewerData = dataResponse.get("viewer").getData();
        var viewerName = viewerData.getDisplayName();
 
        trace('<p><h3>Friends Of "' + viewerName + '":</h3><hr>');
 
        var friendsData = dataResponse.get("viewerFriends").getData();
        friendsData.each(
	        function(friendData) {
	            var friendName = friendData.getDisplayName();
	            var friendThumbnailUrl = friendData.getField(opensocial.Person.Field.THUMBNAIL_URL);
	            trace(friendName + '<br>' + '<image src="' + friendThumbnailUrl + '"/><hr>');
	        }
        );
    }
}
 
function trace(msg) {
    document.getElementById('message').innerHTML += msg + '<br>';
}
 
init();
```


# Request a external site data. #
```
function doMakeRequest()
{
    var param = {};
    gadgets.io.makeRequest("http://deonwu84.appspot.com/foo/myip", gotMakeRequest, param);
}
function gotMakeRequest(response, url, errored)
{
    if(!errored)
    {
		trace(response.text);
    }
    else
    {
		trace("makeRequest failed:(" + response.text + ")(" + url + ")(" + errored + ")" + "\r\n");
    }
}
doMakeRequest();
```
Wiki about makeRequest:
http://wiki.opensocial.org/index.php?title=Introduction_to_makeRequest#What_is_makeRequest.3F