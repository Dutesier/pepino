Feature: Login functionality

  Background:
    Given a user exists with username "user" and password "pass"

  Scenario: Successful login
    Given the user is on the login page
    When the user enters valid credentials
    Then they should be redirected to the dashboard

  Scenario Outline: Unsuccessful login attempts
    Given the user is on the login page
    When the user enters <username> and <password>
    Then they should see an error message

    Examples:
      | username | password |
      | user     | wrongpass |
      | unknown  | pass      |