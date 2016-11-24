
$(document).ready(function() {

  $(document).on('click', '#refresh', function () {
	var data = {};
	$.ajax({
		type: 'POST',
		data: JSON.stringify(data),
        contentType: 'application/json',
        url: 'http://localhost:3000/getlog',						
        success: function(data) {
			//alert(new Date(data['0']['MAX(TIMESTAMP)']));
			var transform = {'<>':'li','html':'(${TIMESTAMP}) (${DEVICEID}) (${MACADR}) (${DISTANCE})'};			
			$('#content').empty();
			$('#content').json2html(data,transform);
			if(data[0]['DISTANCE']<data[1]['DISTANCE'])
			{	
				if(data[0]['DEVICEID']==1)
					$('#content').append( '<tr><td><b>' +'Device: 1 '+ data[0]['MACADR'] + 'Encontra se Sala 1'  + '</b></td></tr>' );
				else
					$('#content').append( '<tr><td><b>' +'Device: 2 '+ data[0]['MACADR'] + 'Encontra se Sala 2'  + '</b></td></tr>' );
			}
			else
			{
				if(data[1]['DEVICEID']==1)
					$('#content').append( '<tr><td><b>' +'Device: 1 '+ data[1]['MACADR'] + ' Encontra se Sala 1'  + '</b></td></tr>' );
				else
					$('#content').append( '<tr><td><b>' +'Device: 2 '+ data[1]['MACADR'] + ' Encontra se Sala 2'  + '</b></td></tr>' );

			}
		}
        });
  });


	window.setInterval(function(){

	var data = {};
	$.ajax({
		type: 'POST',
		data: JSON.stringify(data),
        contentType: 'application/json',
        url: 'http://localhost:3000/getlog',						
        success: function(data) {
			//alert(new Date(data['0']['MAX(TIMESTAMP)']));
			var transform = {'<>':'li','html':'(${TIMESTAMP}) (${DEVICEID}) (${MACADR}) (${DISTANCE})'};			
			$('#content').empty();
			$('#content').json2html(data,transform);
			if(data[0]['DISTANCE']<data[1]['DISTANCE'])
			{	
				if(data[0]['DEVICEID']==1)
					$('#content').append( '<tr><td><b>' +'Device: 1 '+ data[0]['MACADR'] + 'Encontra se Sala 1'  + '</b></td></tr>' );
				else
					$('#content').append( '<tr><td><b>' +'Device: 2 '+ data[0]['MACADR'] + 'Encontra se Sala 2'  + '</b></td></tr>' );
			}
			else
			{
				if(data[1]['DEVICEID']==1)
					$('#content').append( '<tr><td><b>' +'Device: 1 '+ data[1]['MACADR'] + ' Encontra se Sala 1'  + '</b></td></tr>' );
				else
					$('#content').append( '<tr><td><b>' +'Device: 2 '+ data[1]['MACADR'] + ' Encontra se Sala 2'  + '</b></td></tr>' );

			}
		}
		});

	}, 2000);

});
