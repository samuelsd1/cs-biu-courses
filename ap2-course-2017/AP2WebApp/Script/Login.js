//if you already register then go back to home page
if (sessionStorage.username) {
    document.location = 'HomePage.html';
}

/**
 *the main function on this page
 * collect the user input - send to server
 * and act according to the answer.
 * 
*/
(function (angular) {
    var app = angular.module('LogForm', ['jcs-autoValidate']);

    app.run([
        'bootstrap3ElementModifier',
        function (bootstrap3ElementModifier) {
            bootstrap3ElementModifier.enableValidationStateIcons(true);
        }]);
    //send rquest
    app.controller('logCtrl', function ($scope, $http) {
        $scope.model = {};
        $scope.onSubmit = function () {
            var encripted = $.encript(($scope.model.pass));
            var param = {
                UserName: $scope.model.username,
                Password: encripted
            };
            // make the ajax call
            $http({
                method: 'POST',
                url: 'api/Player/Login',
                data: param
            }).then(function successCallback(response) {
                // this callback will be called asynchronously
                // when the response is available
                sessionStorage.username = $scope.model.username;
                window.location.replace("HomePage.html");
            }, function errorCallback(response) {
                // called asynchronously if an error occurs
                // or server returns response with an error status.
                alert('Wrong details.');
            });
        }
    });
}(angular));

/**
 * load the upper nav bar as the page is lodaed
 */
$(document).ready(function () {
    $("#site-header").load("MenuNav.html");
});