//if you already register then go back to home page
if (sessionStorage.username) {
    document.location = 'HomePage.html';
}

/**
 * load the upper nav bar as the page is lodaed
 */
$(document).ready(function () {
    $("#site-header").load("MenuNav.html");
});

/**
 * collect the user input - validate it
 * and go to server to register
 * and act acoording to answer.
*/
(function (angular) {
    var app = angular.module('RegForm', ['jcs-autoValidate']);

    app.run([
        'bootstrap3ElementModifier',
        function (bootstrap3ElementModifier) {
            bootstrap3ElementModifier.enableValidationStateIcons(true);
        }]);
    //make the request from server
    app.controller('regCtrl', function ($scope, $http) {
        $scope.model = {};
        $scope.onSubmit = function () {
            var encripted = $.encript(($scope.model.pass));
            var param = {
                Name: $scope.model.username,
                Email: $scope.model.email,
                Password: encripted,
                Wins: "0",
                Losses: "0"
            };
            // make the ajax call
            $http({
                method: 'POST',
                url: 'api/Player/Register',
                data: param
            }).then(function successCallback(response) {
                // this callback will be called asynchronously
                // when the response is available
                sessionStorage.username = $scope.model.username;
                window.location.replace("HomePage.html");
            }, function errorCallback(response) {
                // called asynchronously if an error occurs
                // or server returns response with an error status.
                alert('A user with that name already exists...');
            });
        }
    });
    /**
     * validates the values
     */
    function ConfirmPasswordValidatorDirective(defaultErrorMessageResolver) {
        defaultErrorMessageResolver.getErrorMessages().then(function (errorMessages) {
            errorMessages['confirmPassword'] = 'Please ensure the passwords match.';
        });

        return {
            restrict: 'A',
            require: 'ngModel',
            scope: {
                confirmPassword: '=confirmPassword'
            },
            link: function (scope, element, attributes, ngModel) {
                ngModel.$validators.confirmPassword = function (modelValue) {
                    return modelValue === scope.confirmPassword;
                };

                scope.$watch('confirmPassword', function () {
                    ngModel.$validate();
                });
            }
        };
    }

    ConfirmPasswordValidatorDirective.$inject = [
        'defaultErrorMessageResolver'
    ];
    app.directive('confirmPassword', ConfirmPasswordValidatorDirective);
}(angular));